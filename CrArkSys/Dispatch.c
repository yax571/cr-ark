#include "ntifs.h"
#include "Dispatch.h"
#include "Enviroment.h"
#include "ProcEnum.h"
#include "Query.h"
#include "Terminate.h"
#include "Protect.h"

//初始化APC
//InputBuffer[0] == ThreadHandle
NTSTATUS
DispatchSpecialInitialize(PVOID InputBuffer, ULONG InputLength,
                          PVOID OutputBuffer, ULONG OutputLength,
                          PULONG Information)
{
    HANDLE threadHandle;
    NTSTATUS status;

    //验证
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


//枚举进程
//InputBuffer[0] == 是否显示Deleting进程
//OutputBuffer ObjectIdTable缓冲
NTSTATUS
DispatchProcessEnum(PVOID InputBuffer, ULONG InputLength,
                    PVOID OutputBuffer, ULONG OutputLength,
                    PULONG Information)
{
    BOOLEAN showDeleting;
    NTSTATUS status;
    PObjectIdTable objIdTable;

    *Information = 0;

    //首先验证用户参数
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

    //枚举进程信息
    objIdTable = ProcessEnum(showDeleting);
    if(!objIdTable)
    {
        status = STATUS_UNSUCCESSFUL;
        return status;
    }

    //拷贝到用户控件
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


//枚举进程
//InputBuffer[0] == 是否显示EPROCESS指针
//OutputBuffer ObjectIdTable缓冲
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

    //枚举线程
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


//\获取进程名称
//InputBuffer[0] == PEPROCESS指针
//OutputBuffer ProcessNameInfo缓冲
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

//获取进程信息
//InputBuffer[0] == PEPROCESS指针
//OutputBuffer ProcessInfo缓冲
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


//获取线程信息
//InputBuffer[0] == PETHREAD指针
//OutputBuffer threadInfo缓冲
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


//获取进程的模块列表
//InputBuffer[0] == PEPROCESS指针
//OutputBuffer ProcessModuleList缓冲
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

//获取进程的模块信息
//InputBuffer[0] == PEPROCESS指针
//InputBuffer[1] == LdrData指针
//OutputBuffer ModuleInfo缓冲
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

//结束线程
//InputBuffer[0] == PETHREAD指针
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

//结束进程
//InputBuffer[0] == PPROCESS指针
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

//卸载Process进程中的指定模块
//InputBuffer[0] == Process      PEPROCESS指针
//InputBuffer[1] == BaseAddress  模块起始地址
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

//保护对象
//InputBuffer[0] == Object
//InputBuffer[1] == 是否删除
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