#include "ntifs.h"
#include "Dispatch.h"
#include "Enviroment.h"
#include "ProcEnum.h"
#include "Query.h"

//��ʼ��APC
//InputBuffer[0] == ThreadHandle
NTSTATUS
DispatchSpecialInitialize(PVOID InputBuffer, ULONG InputLength,
                          PVOID OutputBuffer, ULONG OutputLength,
                          PULONG Information)
{
    HANDLE threadHandle;
    NTSTATUS status;

    //��֤
    if(InputBuffer == NULL ||
       InputLength != sizeof(HANDLE))
       return STATUS_INVALID_PARAMETER;
    *Information = 0;

    status = STATUS_SUCCESS;
    try{
        ProbeForRead(InputBuffer, 4, 1);
        threadHandle = *(HANDLE*)(InputBuffer);
    }except(EXCEPTION_CONTINUE_EXECUTION){
        status = STATUS_ACCESS_VIOLATION;
    }

    if(!NT_SUCCESS(status))
        return status;

    if(!EnviromentSpecialInitialize(threadHandle, NULL, TRUE))
        return STATUS_UNSUCCESSFUL;

    return STATUS_SUCCESS;
}


//ö�ٽ���
//InputBuffer[0] == �Ƿ���ʾDeleting����
//OutputBuffer ObjectIdTable����
NTSTATUS
DispatchProcessEnum(PVOID InputBuffer, ULONG InputLength,
                    PVOID OutputBuffer, ULONG OutputLength,
                    PULONG Information)
{
    BOOLEAN showDeleting;
    NTSTATUS status;
    PObjectIdTable objIdTable;

    *Information = 0;

    //������֤�û�����
    if(InputBuffer == NULL ||
       InputLength != sizeof(BOOLEAN) ||
       OutputBuffer == NULL ||
       OutputLength != sizeof(ObjectIdTable))
    {
        KdPrint(("DispatchProcessEnum Param mismatch: InputLength == %lu(should be %lu), OutputLength == %lu(should be %lu)",
                  InputLength, sizeof(BOOLEAN), OutputLength, sizeof(ObjectIdTable)));
       return STATUS_INVALID_PARAMETER;
    }

    status = STATUS_SUCCESS;
    try{
        ProbeForRead(InputBuffer, InputLength, 1);
        showDeleting = *(BOOLEAN*)InputBuffer;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchProcessEnum ACCESS_VIOLATION read.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
        return status;

    //ö�ٽ�����Ϣ
    objIdTable = ProcessEnum(showDeleting);
    if(!objIdTable)
    {
        status = STATUS_UNSUCCESSFUL;
        return status;
    }

    //�������û��ؼ�
    try{
        ProbeForWrite(OutputBuffer, OutputLength, 1);
        RtlCopyMemory(OutputBuffer, objIdTable, OutputLength);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchProcessEnum ACCESS_VIOLATION write.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }

    ExFreePool(objIdTable);
    if(NT_SUCCESS(status))
        *Information = OutputLength;
    return status;
}


//ö�ٽ���
//InputBuffer[0] == �Ƿ���ʾEPROCESSָ��
//OutputBuffer ObjectIdTable����
NTSTATUS
DispatchThreadEnum(PVOID InputBuffer, ULONG InputLength,
                    PVOID OutputBuffer, ULONG OutputLength,
                    PULONG Information)
{
    PEPROCESS process;
    NTSTATUS status;
    PObjectIdTable objIdTable;

    *Information = 0;
    if(InputBuffer == NULL ||
       InputLength != sizeof(PEPROCESS) ||
       OutputBuffer == NULL ||
       OutputLength != sizeof(ObjectIdTable))
    {
        KdPrint(("DispatchThreadEnum Param mismatch: InputLength == %lu(should be %lu), OutputLength == %lu(should be %lu)",
            InputLength, sizeof(BOOLEAN), OutputLength, sizeof(ObjectIdTable)));
        return STATUS_INVALID_PARAMETER;
    }

    try{
        ProbeForRead(InputBuffer, InputLength, 1);
        process = *(PEPROCESS*)InputBuffer;
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchThreadEnum ACCESS_VIOLATION read.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
        return status;

    //ö���߳�
    objIdTable = ThreadEnum(process);
    if(!objIdTable)
        return STATUS_UNSUCCESSFUL;

    try{
        ProbeForWrite(OutputBuffer, OutputLength, 1);
        RtlCopyMemory(OutputBuffer, objIdTable, OutputLength);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchThreadEnum ACCESS_VIOLATION write.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }   
    
    ExFreePool(objIdTable);
    if(NT_SUCCESS(status))
        *Information = OutputLength;

    return status;
}


//\��ȡ��������
//InputBuffer[0] == PEPROCESSָ��
//OutputBuffer ProcessNameInfo����
NTSTATUS
DispatchQueryProcessName(PVOID InputBuffer, ULONG InputLength,
                         PVOID OutputBuffer, ULONG OutputLength,
                         PULONG Information)
{
    PEPROCESS process;
    NTSTATUS status;
    PProcessNameInfo processNameInfo;

    *Information = 0;

    if(InputBuffer == NULL ||
       InputLength != sizeof(PEPROCESS) ||
       OutputBuffer == NULL ||
       OutputLength != sizeof(ProcessNameInfo))
    {
        KdPrint(("DispatchQueryProcessName Param mismatch: InputLength == %lu(should be %lu), OutputLength == %lu(should be %lu)",
            InputLength, sizeof(BOOLEAN), OutputLength, sizeof(ObjectIdTable)));
        return STATUS_INVALID_PARAMETER;
    }

    try{
        ProbeForRead(InputBuffer, InputLength, 1);
        process = *(PEPROCESS*)InputBuffer;
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
        return status;

    processNameInfo = QueryProcessName(process);
    if(processNameInfo == NULL)
        return STATUS_UNSUCCESSFUL;

    try{
        ProbeForWrite(OutputBuffer, OutputLength, 1);
        RtlCopyMemory(OutputBuffer, processNameInfo, OutputLength);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        status = STATUS_ACCESS_VIOLATION;
    }

    ExFreePool(processNameInfo);
    if(!NT_SUCCESS(status))
        *Information = OutputLength;

    return status;
}

//��ȡ������Ϣ
//InputBuffer[0] == PEPROCESSָ��
//OutputBuffer ProcessInfo����
NTSTATUS
DispatchQueryProcessInfo(PVOID InputBuffer, ULONG InputLength,
                         PVOID OutputBuffer, ULONG OutputLength,
                         PULONG Information)
{
    NTSTATUS status;
    PEPROCESS process;
    PProcessInfo processInfo;

    *Information = 0;

    if(InputBuffer == NULL ||
       InputLength != sizeof(PEPROCESS) ||
       OutputBuffer == NULL ||
       OutputLength != sizeof(ProcessInfo))
    {
        KdPrint(("DispatchQueryProcessInfo Param mismatch: InputLength == %lu(should be %lu), OutputLength == %lu(should be %lu)",
            InputLength, sizeof(BOOLEAN), OutputLength, sizeof(ObjectIdTable)));
       return STATUS_INVALID_PARAMETER;
    }

    try{
        ProbeForRead(InputBuffer, InputLength, 1);
        process = *(PEPROCESS*)InputBuffer;
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
    {
        KdPrint(("DispatchQueryProcessInfo ACCESS_VIOLATION read.\n"));
        return status;
    }

    processInfo = QueryProcessInfo(process);
    if(processInfo == NULL)
        return STATUS_UNSUCCESSFUL;

    try{
        ProbeForWrite(OutputBuffer, OutputLength, 1);
        RtlCopyMemory(OutputBuffer, processInfo, OutputLength);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchQueryProcessInfo ACCESS_VIOLATION write.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }

    if(NT_SUCCESS(status))
        *Information = OutputLength;

    ExFreePool(processInfo);
    return status;
}