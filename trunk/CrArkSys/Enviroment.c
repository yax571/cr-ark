#include <ntifs.h>
#include "Enviroment.h"
#include "ade32.h"
#include "Undocument.h"
#include "helper.h"

//KRPCESS偏移
ULONG KProcessBasePriorityOffset;
ULONG KProcessStateOffset;

//EPROCESS结构中的偏移
ULONG EProcessUniqueProcessIdOffset;
ULONG EProcessActiveProcessLinksOffset;
ULONG EProcessUniqueProcessIdOffset;
ULONG EProcessObjectTableOffset;
ULONG EProcessSectionObjectOffset;
ULONG EProcessImageFileNameOffset;
ULONG EProcessThreadListHeadOffset;
ULONG EProcessObjectTableOffset;
ULONG EProcessSectionObjectOffset;
ULONG EProcessPebOffset;
ULONG EProcessCreateTimeOffset;
ULONG EProcessDebugPortOffset;
ULONG EProcessInheritedFromUniqueProcessIdOffset;


//HandleTable结构中的偏移
ULONG HandleTableHandleTableListOffset;
ULONG HandleTableQuotaProcessOffset;
ULONG HandleTableUniqueProcessIdOffset;
ULONG HandleTableHandleCountOffset;

//EThread结构中的偏移
ULONG EThreadCidOffset;
ULONG EThreadApcStateProcessOffset;
ULONG EThreadThreadListEntryOffset;
ULONG EThreadStartAddressOffset;
ULONG EThreadWin32StartAddressOffset;
ULONG KThreadContextSwitchesOffset;
ULONG KThreadBasePriorityOffset;
ULONG KThreadStateOffset;
ULONG KThreadPreviousModeOffset;

//SectionObject偏移
ULONG SectionObjectSegmentOffset;
ULONG SegmentObjectSubSecOffset;
ULONG ControlAreaFilePointerOffset;

//动态获得的地址
PVOID SystemProcess;
PVOID PspCidTable;
ULONG NtKernelBase;
ULONG NtKernelSize;
PLIST_ENTRY ActiveProcessLinksHead;
PLIST_ENTRY PsLoadedModuleList;
PLIST_ENTRY HandleTableListHead;

//用于终止线程的APC-Routine
PVOID PsExitSpecialApc;
PVOID PspExitApcRundown;
PVOID PspExitNormalApc;

PETHREAD EThreadForGetApc;
UCHAR KeInsertQueueApcJumpBack[20];

//动态获得的函数
NTSTATUS
(__stdcall *NtTerminateThread) (
                      __in_opt HANDLE ThreadHandle,
                      __in NTSTATUS ExitStatus
                      );

NTSTATUS
(__stdcall *NtTerminateProcess)(
                   __in_opt HANDLE ProcessHandle,
                   __in NTSTATUS ExitStatus
                   );

//搜索PsLookProcessThreadByCide函数, 取得未导出的PspCidTable地址
BOOLEAN
GetPspCidTableAddress()
{
    ULONG length, functionAddress, current;
    UNICODE_STRING functionName;

    //获得PsLookupProcessThreadByCid路径
    RtlInitUnicodeString(&functionName, 
                         L"PsLookupProcessThreadByCid");
    functionAddress = (ULONG)MmGetSystemRoutineAddress(&functionName);
    if(functionAddress == 0)
        return FALSE;

    //搜索 push    _PspCidTable指令, 获得PspCidTable地址
    current = functionAddress;
    length = 0;
    do{
        current += length;
        length = ade32_disasm((PVOID)current);
        if(length == 6 && (*(USHORT*)current == 0x35ff || *(USHORT*)current == 0x3d8b))
        {
            current += 2;
            PspCidTable = *(PVOID*)(*(ULONG*)current);
            return TRUE;
        }
    }while(length != 0 && current <= functionAddress + 0x100);

    return FALSE; 
}

//暴力搜索PsLoadedModuleListHead的地址
BOOLEAN
GetPsLoadedModuleListHead(PDRIVER_OBJECT DriverObject)
{
    PLDR_DATA_TABLE_ENTRY entry, current;
    PSYSTEM_MODULE_INFORMATION moduleInfos;
    NTSTATUS status;
    ULONG retLen = 0;
    PVOID pool;

    entry = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
    current = (PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Blink;

    while(current != entry)
    {
        //如果current是头, 基本可以落入这些判断条件
        // kd> dd PsLoadedModuleList
      //       805541a0  821fc398 82058b90 00000000 00000000
      //       805541b0  00000000 00000000 00000000 00000000
      //       805541c0  8055b560 80558860 00000000 00000000
      //       805541d0  00000000 00000000 00000000 00000000
      //       805541e0  00000000 00000000 00000000 00000000
        if((ULONG)current->DllBase < 0x8000000 ||
            current->EntryPoint == 0x0 ||
            current->SizeOfImage > 0x20000000 ||
            current->EntryPoint <= current->DllBase ||
            (ULONG)current->EntryPoint >= (ULONG)current->DllBase + current->SizeOfImage)
        {
            PsLoadedModuleList = (PLIST_ENTRY)current;
            //现在获得内核基址, InLoadOrderLinks第一项是内核
            current = (PLDR_DATA_TABLE_ENTRY)current->InLoadOrderLinks.Flink;
            NtKernelBase = (ULONG)current->DllBase;
            NtKernelSize = (ULONG)current->SizeOfImage;

            //验证是否正确
            status = ZwQuerySystemInformation(SystemModuleInformation,
                                              NULL, 0, &retLen);
            if(retLen == 0)
                return FALSE;
            pool = ExAllocatePoolWithTag(NonPagedPool,
                                         retLen, 'smin');
            if(pool == NULL)
                return FALSE;
            status = ZwQuerySystemInformation(SystemModuleInformation,
                                              pool,
                                              retLen,
                                              &retLen);
            if(!NT_SUCCESS(status))
            {
                ExFreePool(pool);
                return FALSE;
            }
            moduleInfos = (PSYSTEM_MODULE_INFORMATION)((ULONG)pool + 4);
            if((ULONG)moduleInfos->Base == NtKernelBase &&
               moduleInfos->Size == NtKernelSize)
            {
                ExFreePool(pool);
                return TRUE;
            }
            else
            {
                ExFreePool(pool);
                return FALSE;
            }
        }
        current = (PLDR_DATA_TABLE_ENTRY)current->InLoadOrderLinks.Blink;
    }

    return FALSE;
}

//这个函数要使用内核基址, 所以要放到GetPsLoadedModuleListHead之后调用
BOOLEAN
GetActiveProcessLinksHead()
{
    PLIST_ENTRY entry, next;
    BOOLEAN found;
    KIRQL oldIrql;

    entry = (PLIST_ENTRY)((PUCHAR)SystemProcess + EProcessActiveProcessLinksOffset);
    found = FALSE;

    //一般来讲System是系统中第一个进程
    //这个进程的前一项就是列表头
    oldIrql = KeRaiseIrqlToDpcLevel();
    next = entry->Blink;
    while(next != entry)
    {
        //如果当前列表在内核中就是ActiveProcessLinks头了
        if((ULONG)next < NtKernelBase + NtKernelSize &&
            (ULONG)next > NtKernelBase)
        {
            ActiveProcessLinksHead = next;
            found = TRUE;
            break;
        }
        next = next->Blink;
    }
    KeLowerIrql(oldIrql);

    return found;
}

BOOLEAN
GetHandleTableListHead()
{
    PLIST_ENTRY entry, next;
    KIRQL oldIrql;
    BOOLEAN  found;

    found = FALSE;
    oldIrql = KeRaiseIrqlToDpcLevel();

    entry = (PLIST_ENTRY)(*(PULONG)((ULONG)SystemProcess + EProcessObjectTableOffset));
    entry = (PLIST_ENTRY)((ULONG)entry + HandleTableHandleTableListOffset);
    next = entry->Blink;

    while(next != entry)
    {
        if((ULONG)next >= NtKernelBase &&
            (ULONG)next < NtKernelBase + NtKernelSize)
        {
            HandleTableListHead = next;
            found = TRUE;
            break;
        }
        next = next->Blink;
    }

    KeLowerIrql(oldIrql);
    return found;
}

BOOLEAN
GetServicesFunction()
{
    ULONG buildNo;
    ULONG ZwTerminateThreadNo;
    ULONG ZwTerminateProcessNo;

    PsGetVersion(NULL, NULL, &buildNo, NULL);

    switch(buildNo)
    {
    case 2600:          //winxp
        ZwTerminateProcessNo = 0x101;
        ZwTerminateThreadNo = 0x102;
        break;
    }

    *(PULONG)(&NtTerminateThread) = KeServiceDescriptorTable.ServiceTable[ZwTerminateThreadNo];
    *(PULONG)(&NtTerminateProcess) = KeServiceDescriptorTable.ServiceTable[ZwTerminateProcessNo];
    return TRUE;
}

//初始化一些偏移量
BOOLEAN
EnviromentInitialize(PDRIVER_OBJECT DriverObject)
{
    ULONG buildNo;

    PsGetVersion(NULL, NULL, &buildNo, NULL);

    switch(buildNo)
    {
    case 2600:      //winxp
        EProcessUniqueProcessIdOffset = 0x084;
        EProcessActiveProcessLinksOffset = 0x088;
        EProcessObjectTableOffset = 0x0c4;
        EProcessSectionObjectOffset = 0x138;
        EProcessImageFileNameOffset = 0x174;
        EProcessThreadListHeadOffset = 0x190;
        EProcessObjectTableOffset = 0x0c4;
        EProcessPebOffset = 0x1b0;
        EProcessCreateTimeOffset = 0x070;
        EProcessDebugPortOffset = 0x0bc;
        EProcessInheritedFromUniqueProcessIdOffset = 0x14c;

        EThreadCidOffset = 0x1ec;
        EThreadApcStateProcessOffset = 0x044;
        EThreadThreadListEntryOffset = 0x22c;
        EThreadStartAddressOffset = 0x224;
        EThreadWin32StartAddressOffset = 0x228;

        KThreadBasePriorityOffset = 0x6c;
        KThreadContextSwitchesOffset = 0x04c;
        KThreadStateOffset = 0x02d;
        KThreadPreviousModeOffset = 0x140;

        KProcessBasePriorityOffset = 0x062;
        KProcessStateOffset = 0x065;

        HandleTableHandleTableListOffset = 0x01c;
        HandleTableQuotaProcessOffset = 0x004;
        HandleTableUniqueProcessIdOffset = 0x08;
        HandleTableHandleCountOffset = 0x03c;

        SectionObjectSegmentOffset = 0x014;
        SegmentObjectSubSecOffset = 0x01c;
        ControlAreaFilePointerOffset = 0x024;
        break;

    default:
        return FALSE;
    }
    
    //获得System进程
    SystemProcess = PsGetCurrentProcess();
    //获得未导出的函数
    if(GetServicesFunction() == FALSE)
        return FALSE;
    //获得PsLoadedModuleList地址和内核基址、大小
    if(GetPsLoadedModuleListHead(DriverObject) == FALSE)
        return FALSE;
    //获得ActiveProcessLinks头
    if(GetActiveProcessLinksHead() == FALSE)
        return FALSE;
    //获得HandleTableList头
    if(GetHandleTableListHead() == FALSE)
        return FALSE;
    //获得PspCidTable地址
    if(GetPspCidTableAddress() == FALSE)
        return FALSE;

    return TRUE;
}


BOOLEAN
FakeKeInsertQueueApc (
                  __inout PRKAPC Apc,
                  __in_opt PVOID SystemArgument1,
                  __in_opt PVOID SystemArgument2,
                  __in KPRIORITY Increment
                  )
{
    ULONG retVal;

    //获取PsExitSpecialApc、PspExitApcRundown、PspExitNormalApc
    if(EThreadForGetApc != NULL &&
       Apc->ApcMode == KernelMode &&
       Apc->NormalContext == (PVOID)0x12345678 &&
       Apc->Thread == (PKTHREAD)EThreadForGetApc &&
       Apc->ApcStateIndex == 0 /*OriginalApcEnvironment*/)
    {
        PsExitSpecialApc = (PVOID)Apc->KernelRoutine;
        PspExitApcRundown = (PVOID)Apc->RundownRoutine;
        PspExitNormalApc = (PVOID)Apc->NormalRoutine;

        //获得成功后取消钩子
        EThreadForGetApc = NULL;
        UnhookFunction(KeInsertQueueApc, KeInsertQueueApcJumpBack);
        KdPrint(("get PsExitSpecialApc: %8.8X, PspExitApcRundown: %8.8X, PspExitNormalApc: %8.8X\n",
                 PsExitSpecialApc, PspExitApcRundown, PspExitNormalApc));
        return FALSE;
    }
    
    __asm {
        push Increment;
        push SystemArgument2;
        push SystemArgument1;
        push Apc;
        lea eax, KeInsertQueueApcJumpBack;
        call eax;
        mov retVal, eax;
    }

    return retVal > 0;
}

//这个函数主要用于确定线程结束时的APC参数
//由于实现关系, ThreadHandle/Thread必须为有效的非系统线程句柄/指针
//Handle  为TRUE时使用ThreadHandle
//        为FALSE使用Thread
BOOLEAN
EnviromentSpecialInitialize(HANDLE ThreadHandle, PVOID Thread, BOOLEAN Handle)
{
    BOOLEAN retVal;
    PETHREAD thread;
    NTSTATUS status;
    KPROCESSOR_MODE previousMode;

    //已经初始化了
    if(PsExitSpecialApc ||
       PspExitApcRundown ||
       PspExitNormalApc)
    {
        return TRUE;
    }

    if(Handle)
    {
        status = ObReferenceObjectByHandle(ThreadHandle,
                                           THREAD_TERMINATE,
                                           *PsThreadType,
                                           KernelMode,
                                           &thread,
                                           NULL);
        if(!NT_SUCCESS(status))
            return FALSE;
        ObDereferenceObject(thread);
    }
    else
    {
        thread = Thread;
        status = ObOpenObjectByPointer(Thread,
                                       OBJ_KERNEL_HANDLE,
                                       NULL,
                                       0,
                                       *PsThreadType,
                                       KernelMode,
                                       &ThreadHandle);
        if(!NT_SUCCESS(status))
            return FALSE;
    }

    EThreadForGetApc = thread;

    retVal = HookFunction(KeInsertQueueApc, 
                          FakeKeInsertQueueApc, 
                          KeInsertQueueApcJumpBack);
    if(!retVal)
    {
        EThreadForGetApc = NULL;
        return retVal;
    }
    
    previousMode = SetCurrentThreadProcessorMode(KernelMode);
    NtTerminateThread(ThreadHandle, 0x12345678);
    SetCurrentThreadProcessorMode(previousMode);


    if(!Handle)
        ZwClose(ThreadHandle);

    return TRUE;
}
