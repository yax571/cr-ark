#include <ntifs.h>
#include "helper.h"
#include "Enviroment.h"
#include "Undocument.h"
#include "ProcEnum.h"

//普通方式终止线程
BOOLEAN
TerminateThread(PETHREAD Thread,
                NTSTATUS ExitStatus)
{
    HANDLE threadHandle;
    NTSTATUS status;
    KPROCESSOR_MODE previousMode;

    if(!IsThreadObject(Thread))
        return FALSE;

    status = ObOpenObjectByPointer(Thread, OBJ_KERNEL_HANDLE,
                                   NULL, THREAD_TERMINATE,
                                   *PsThreadType, KernelMode,
                                   &threadHandle);
    if(!NT_SUCCESS(status))
        return FALSE;

    previousMode = SetCurrentThreadProcessorMode(KernelMode);
    status = NtTerminateThread(threadHandle, ExitStatus);
    SetCurrentThreadProcessorMode(previousMode);

    ZwClose(threadHandle);
    return NT_SUCCESS(status);
}


//使用APC终止线程, 参考WRK内部实现
BOOLEAN
ForceTerminateThread(PETHREAD Thread,
                     NTSTATUS ExitStatus)
{
    PKAPC exitApc;
    LARGE_INTEGER shortTime = {(ULONG)(-10 * 1000 * 100), -1};

    if(!IsThreadObject(Thread))
        return FALSE;

    if(Thread == KeGetCurrentThread())
        return FALSE;

    while(1) {
        exitApc = (PKAPC)ExAllocatePoolWithTag(NonPagedPool,
                                               sizeof(KAPC),
                                               'xEsP');
        if(exitApc != NULL)
            break;
        KeDelayExecutionThread(KernelMode, FALSE, &shortTime);
    }

    KeInitializeApc(exitApc, Thread, 0, 
                    PsExitSpecialApc,
                    PspExitApcRundown,
                    PspExitNormalApc,
                    KernelMode,
                    (PVOID)ExitStatus);
    if(!KeInsertQueueApc(exitApc, exitApc, NULL, 2)) {
        ExFreePool(exitApc);
        return FALSE;
    }
    
    return TRUE;
}

//普通方式结束进程
BOOLEAN
TerminateProcess(PEPROCESS Process, 
                 NTSTATUS ExitCode )
{
    NTSTATUS status;
    HANDLE handle;
    KPROCESSOR_MODE previousMode;

    if(!IsProcessObject(Process))
        return FALSE;

    status = ObOpenObjectByPointer(Process, OBJ_KERNEL_HANDLE,
                                   NULL, 0x01, //PROCESS_TERMINATE,
                                   *PsProcessType, KernelMode,
                                   &handle);
    if(!NT_SUCCESS(status))
        return FALSE;

    previousMode = SetCurrentThreadProcessorMode(KernelMode);
    status = NtTerminateProcess(handle, ExitCode);
    SetCurrentThreadProcessorMode(previousMode);

    ZwClose(handle);
    return NT_SUCCESS(status);
}

BOOLEAN
ForceTerminateProcess(PEPROCESS Process,
                      NTSTATUS ExitStatus)
{
    PObjectIdTable threadTable;
    ULONG i;

    threadTable = ThreadEnum(Process);
    if(threadTable == NULL)
        return FALSE;

    for(i = 0; i < threadTable->Count; i++)
    {
        ForceTerminateThread((PETHREAD)threadTable->Entry[i].Object,
                              ExitStatus);
    }
    ExFreePool(threadTable);

    return TRUE;
}


//卸载Process进程中的以BaseAddress为起始地址的模块
BOOLEAN
UnmapProcessModule(PEPROCESS Process, PVOID BaseAddress)
{
    NTSTATUS status;
    HANDLE processHandle;

    if(!IsProcessObject(Process))
        return FALSE;

    if(Process == PsGetCurrentProcess())
        return FALSE;

    status = ObOpenObjectByPointer(Process, OBJ_KERNEL_HANDLE,
                                   NULL, 0,
                                   *PsProcessType, KernelMode,
                                   &processHandle);
    if(!NT_SUCCESS(status))
        return FALSE;

    status = ZwUnmapViewOfSection(processHandle, BaseAddress);
    ZwClose(processHandle);

    return NT_SUCCESS(status);
}