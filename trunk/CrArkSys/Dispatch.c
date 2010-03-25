#include "ntifs.h"
#include "Dispatch.h"
#include "Enviroment.h"
#include "ProcEnum.h"
#include "Query.h"
#include "Terminate.h"
#include "Protect.h"

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
            InputLength, sizeof(PEPROCESS), OutputLength, sizeof(ObjectIdTable)));
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
            InputLength, sizeof(PEPROCESS), OutputLength, sizeof(ProcessNameInfo)));
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
            InputLength, sizeof(PEPROCESS), OutputLength, sizeof(ProcessInfo)));
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


//��ȡ�߳���Ϣ
//InputBuffer[0] == PETHREADָ��
//OutputBuffer threadInfo����
NTSTATUS
DispatchQueryThreadInfo(PVOID InputBuffer, ULONG InputLength,
                        PVOID OutputBuffer, ULONG OutputLength,
                        PULONG Information)
{
    NTSTATUS status;
    PETHREAD thread;
    PThreadInfo threadInfo;

    *Information = 0;
    if(InputBuffer == NULL ||
       InputLength != sizeof(PETHREAD) ||
       OutputBuffer == NULL ||
       OutputLength != sizeof(ThreadInfo))
    {
        KdPrint(("DispatchQueryThreadInfo Param mismatch: InputLength == %lu(should be %lu), OutputLength == %lu(should be %lu)",
            InputLength, sizeof(PETHREAD), OutputLength, sizeof(ThreadInfo)));
        return STATUS_INVALID_PARAMETER;
    }


    try{
        ProbeForRead(InputBuffer, InputLength, 1);
        thread = *(PETHREAD*)(InputBuffer);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
    {
        KdPrint(("DispatchQueryThreadInfo ACCESS_VIOLATION read.\n"));
        return status;
    }

    threadInfo = QueryThreadInfo(thread);
    if(threadInfo == NULL)
        return STATUS_UNSUCCESSFUL;

    try{
        ProbeForWrite(OutputBuffer, OutputLength, 1);
        RtlCopyMemory(OutputBuffer, threadInfo, OutputLength);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchQueryThreadInfo ACCESS_VIOLATION write.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }

    if(NT_SUCCESS(status))
        *Information = OutputLength;
    ExFreePool(threadInfo);
    return status;
}


//��ȡ���̵�ģ���б�
//InputBuffer[0] == PEPROCESSָ��
//OutputBuffer ProcessModuleList����
NTSTATUS
DispatchQueryProcessModuleList(PVOID InputBuffer, ULONG InputLength,
                               PVOID OutputBuffer, ULONG OutputLength,
                               PULONG Information)
{
    NTSTATUS status;
    PEPROCESS process;
    PProcessModuleList processModList;

    *Information = 0;
    if(InputBuffer == NULL ||
       InputLength != sizeof(PEPROCESS) ||
       OutputBuffer == NULL ||
       OutputLength != sizeof(ProcessModuleList))
    {
        KdPrint(("DispatchQueryProcessModuleList Param mismatch: InputLength == %lu(should be %lu), OutputLength == %lu(should be %lu)",
            InputLength, sizeof(PEPROCESS), OutputLength, sizeof(ProcessModuleList)));
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
        KdPrint(("DispatchQueryProcessModuleList ACCESS_VIOLATION read.\n"));
        return status;
    }

    processModList = QueryProcessModuleList(process);
    if(processModList == NULL)
        return STATUS_UNSUCCESSFUL;

    try{
        ProbeForWrite(OutputBuffer, OutputLength, 1);
        RtlCopyMemory(OutputBuffer, processModList, OutputLength);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchQueryProcessModuleList ACCESS_VIOLATION write.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }

    if(NT_SUCCESS(status))
        *Information = OutputLength;
    ExFreePool(processModList);
    return status;
}

//��ȡ���̵�ģ����Ϣ
//InputBuffer[0] == PEPROCESSָ��
//InputBuffer[1] == LdrDataָ��
//OutputBuffer ModuleInfo����
NTSTATUS
DispatchQueryProcessModuleInfo(PVOID InputBuffer, ULONG InputLength,
                               PVOID OutputBuffer, ULONG OutputLength,
                               PULONG Information)
{
    NTSTATUS status;
    PEPROCESS process;
    PVOID ldrData;
    PModuleInfo modInfo;

    *Information = 0;
    if(InputBuffer == NULL ||
        InputLength != sizeof(PEPROCESS) + sizeof(PVOID)||
        OutputBuffer == NULL ||
        OutputLength != sizeof(ModuleInfo))
    {
        KdPrint(("DispatchQueryProcessModuleInfo Param mismatch: InputLength == %lu(should be %lu), OutputLength == %lu(should be %lu)",
            InputLength, sizeof(PEPROCESS) + sizeof(PVOID), OutputLength, sizeof(ModuleInfo)));
        return STATUS_INVALID_PARAMETER;
    }

    try{
        ProbeForRead(InputBuffer, InputLength, 1);
        process = *(PEPROCESS*)InputBuffer;
        ldrData = *(PVOID*)((ULONG)InputBuffer + 4);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
    {
        KdPrint(("DispatchQueryProcessModuleInfo ACCESS_VIOLATION read.\n"));
        return status;
    }

    modInfo = QueryModuleInfo(process, ldrData);
    if(modInfo == NULL)
        return STATUS_UNSUCCESSFUL;

    try{
        ProbeForWrite(OutputBuffer, OutputLength, 1);
        RtlCopyMemory(OutputBuffer, modInfo, OutputLength);
        status = STATUS_SUCCESS;
    }except(EXCEPTION_CONTINUE_EXECUTION){
        KdPrint(("DispatchQueryProcessModuleInfo ACCESS_VIOLATION write.\n"));
        status = STATUS_ACCESS_VIOLATION;
    }

    if(NT_SUCCESS(status))
        *Information = OutputLength;
    ExFreePool(modInfo);
    return status;
}

//�����߳�
//InputBuffer[0] == PETHREADָ��
//InputBuffer[1] == ExitStatus
//InputBuffer[2] == ForceExit
NTSTATUS
DispatchTerminateThread(PVOID InputBuffer, ULONG InputLength,
                        PVOID OutputBuffer, ULONG OutputLength,
                        PULONG Information)
{
    NTSTATUS status;
    PETHREAD thread;
    NTSTATUS exitStatus;
    BOOLEAN forceExit, success;

    *Information = 0;

    if(InputBuffer == NULL ||
       InputLength != sizeof(ULONG) * 3)
    {
        KdPrint(("DispatchTerminateThread Param length mismatch\n"));
        return STATUS_INVALID_PARAMETER;
    }

    try {
        ProbeForRead(InputBuffer, InputLength, 1);
        thread = *(PETHREAD*)InputBuffer;
        exitStatus = *(NTSTATUS*)((PULONG)InputBuffer + 4);
        forceExit = *(BOOLEAN*)((PULONG)InputBuffer + 8);
        status = STATUS_SUCCESS;
    } except(EXCEPTION_CONTINUE_EXECUTION) {
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
        return status;

    if(forceExit) {
        success = ForceTerminateThread(thread, exitStatus);
    }
    else {
        success = TerminateThread(thread, exitStatus);
    }
    
    if(success)
        return STATUS_SUCCESS;
    else
        return STATUS_UNSUCCESSFUL;
}

//��������
//InputBuffer[0] == PPROCESSָ��
//InputBuffer[1] == ExitStatus
//InputBuffer[2] == ForceExit
NTSTATUS
DispatchTerminateProcess(PVOID InputBuffer, ULONG InputLength,
                         PVOID OutputBuffer, ULONG OutputLength,
                         PULONG Information)
{
    NTSTATUS status;
    PEPROCESS process;
    NTSTATUS exitStatus;
    BOOLEAN forceExit;

    *Information = 0;

    if(InputBuffer == NULL ||
        InputLength != sizeof(ULONG) * 3)
    {
        KdPrint(("DispatchTerminateProcess Param length mismatch\n"));
        return STATUS_INVALID_PARAMETER;
    }

    try{
        ProbeForRead(InputBuffer, InputLength, 1);
        process = *(PEPROCESS*)InputBuffer;
        exitStatus = *(NTSTATUS*)((ULONG)InputBuffer + 4);
        forceExit = *(BOOLEAN*)((ULONG)InputBuffer + 8);
        status = STATUS_SUCCESS;
    } except(EXCEPTION_CONTINUE_EXECUTION) {
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
        return status;

    if(forceExit)
        ForceTerminateProcess(process, exitStatus);
    else
        TerminateProcess(process, exitStatus);

    return STATUS_SUCCESS;
}

//ж��Process�����е�ָ��ģ��
//InputBuffer[0] == Process      PEPROCESSָ��
//InputBuffer[1] == BaseAddress  ģ����ʼ��ַ
NTSTATUS
DispatchUnmapProcessModule(PVOID InputBuffer, ULONG InputLength,
                           PVOID OutputBuffer, ULONG OutputLength,
                           PULONG Information)
{
    PEPROCESS process;
    PVOID baseAddress;
    NTSTATUS status;
    BOOLEAN bRet;

    *Information = 0;

    if(InputBuffer == NULL ||
       InputLength != sizeof(ULONG) * 2)
    {
        KdPrint(("DispatchUnmapProcessModule Param length mismatch\n"));
        return STATUS_INVALID_PARAMETER;
    }

    try {
        ProbeForRead(InputBuffer, sizeof(ULONG) * 2, 1);
        process = *(PEPROCESS*)InputBuffer;
        baseAddress = *(PVOID*)((ULONG)InputBuffer + 4);
        status = STATUS_SUCCESS;
    } except(EXCEPTION_CONTINUE_EXECUTION) {
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
        return status;

    bRet = UnmapProcessModule(process, baseAddress);

    return STATUS_SUCCESS;
}

//��������
//InputBuffer[0] == Object
//InputBuffer[1] == �Ƿ�ɾ��
NTSTATUS
DispatchProtectObject(PVOID InputBuffer, ULONG InputLength,
                      PVOID OutputBuffer, ULONG OutputLength,
                      PULONG Information)
{
    PVOID object;
    BOOLEAN remove;
    NTSTATUS status;

    *Information = 0;

    if(InputBuffer == NULL ||
       InputLength != sizeof(ULONG) * 2)
    {
        KdPrint(("DispatchProtectObject Param length mismatch\n"));
        return STATUS_INVALID_PARAMETER;
    }

    try {
        ProbeForRead(InputBuffer, sizeof(ULONG) * 2, 1);
        object = *(PVOID*)InputBuffer;
        remove = (BOOLEAN)(*(PULONG)((ULONG)InputBuffer + 4));
        status = STATUS_SUCCESS;
    } except(EXCEPTION_CONTINUE_EXECUTION) {
        status = STATUS_ACCESS_VIOLATION;
    }
    if(!NT_SUCCESS(status))
        return status;

    ProtectAddObject(object, remove);
    return status;
}