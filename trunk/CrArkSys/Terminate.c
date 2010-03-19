#include <ntifs.h>
#include "helper.h"
#include "Enviroment.h"
#include "Undocument.h"

//��ͨ��ʽ��ֹ�߳�
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


//ʹ��APC��ֹ�߳�, �ο�WRK�ڲ�ʵ��
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