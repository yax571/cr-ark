#include <ntifs.h>
#include "Enviroment.h"
#include "ade32.h"
#include "Undocument.h"
#include "helper.h"

//KRPCESSƫ��
ULONG KProcessBasePriorityOffset;
ULONG KProcessStateOffset;

//EPROCESS�ṹ�е�ƫ��
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


//HandleTable�ṹ�е�ƫ��
ULONG HandleTableHandleTableListOffset;
ULONG HandleTableQuotaProcessOffset;
ULONG HandleTableUniqueProcessIdOffset;
ULONG HandleTableHandleCountOffset;

//EThread�ṹ�е�ƫ��
ULONG EThreadCidOffset;
ULONG EThreadApcStateProcessOffset;
ULONG EThreadThreadListEntryOffset;
ULONG EThreadStartAddressOffset;
ULONG EThreadWin32StartAddressOffset;
ULONG KThreadContextSwitchesOffset;
ULONG KThreadBasePriorityOffset;
ULONG KThreadStateOffset;
ULONG KThreadPreviousModeOffset;

//SectionObjectƫ��
ULONG SectionObjectSegmentOffset;
ULONG SegmentObjectSubSecOffset;
ULONG ControlAreaFilePointerOffset;

//��̬��õĵ�ַ
PVOID SystemProcess;
PVOID PspCidTable;
ULONG NtKernelBase;
ULONG NtKernelSize;
PLIST_ENTRY ActiveProcessLinksHead;
PLIST_ENTRY PsLoadedModuleList;
PLIST_ENTRY HandleTableListHead;

//������ֹ�̵߳�APC-Routine
PVOID PsExitSpecialApc;
PVOID PspExitApcRundown;
PVOID PspExitNormalApc;

PETHREAD EThreadForGetApc;
UCHAR KeInsertQueueApcJumpBack[20];

//��̬��õĺ���
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

//����PsLookProcessThreadByCide����, ȡ��δ������PspCidTable��ַ
BOOLEAN
GetPspCidTableAddress()
{
    ULONG length, functionAddress, current;
    UNICODE_STRING functionName;

    //���PsLookupProcessThreadByCid·��
    RtlInitUnicodeString(&functionName, 
                         L"PsLookupProcessThreadByCid");
    functionAddress = (ULONG)MmGetSystemRoutineAddress(&functionName);
    if(functionAddress == 0)
        return FALSE;

    //���� push    _PspCidTableָ��, ���PspCidTable��ַ
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

//��������PsLoadedModuleListHead�ĵ�ַ
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
        //���current��ͷ, ��������������Щ�ж�����
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
            //���ڻ���ں˻�ַ, InLoadOrderLinks��һ�����ں�
            current = (PLDR_DATA_TABLE_ENTRY)current->InLoadOrderLinks.Flink;
            NtKernelBase = (ULONG)current->DllBase;
            NtKernelSize = (ULONG)current->SizeOfImage;

            //��֤�Ƿ���ȷ
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

//�������Ҫʹ���ں˻�ַ, ����Ҫ�ŵ�GetPsLoadedModuleListHead֮�����
BOOLEAN
GetActiveProcessLinksHead()
{
    PLIST_ENTRY entry, next;
    BOOLEAN found;
    KIRQL oldIrql;

    entry = (PLIST_ENTRY)((PUCHAR)SystemProcess + EProcessActiveProcessLinksOffset);
    found = FALSE;

    //һ������System��ϵͳ�е�һ������
    //������̵�ǰһ������б�ͷ
    oldIrql = KeRaiseIrqlToDpcLevel();
    next = entry->Blink;
    while(next != entry)
    {
        //�����ǰ�б����ں��о���ActiveProcessLinksͷ��
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

//��ʼ��һЩƫ����
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
    
    //���System����
    SystemProcess = PsGetCurrentProcess();
    //���δ�����ĺ���
    if(GetServicesFunction() == FALSE)
        return FALSE;
    //���PsLoadedModuleList��ַ���ں˻�ַ����С
    if(GetPsLoadedModuleListHead(DriverObject) == FALSE)
        return FALSE;
    //���ActiveProcessLinksͷ
    if(GetActiveProcessLinksHead() == FALSE)
        return FALSE;
    //���HandleTableListͷ
    if(GetHandleTableListHead() == FALSE)
        return FALSE;
    //���PspCidTable��ַ
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

    //��ȡPsExitSpecialApc��PspExitApcRundown��PspExitNormalApc
    if(EThreadForGetApc != NULL &&
       Apc->ApcMode == KernelMode &&
       Apc->NormalContext == (PVOID)0x12345678 &&
       Apc->Thread == (PKTHREAD)EThreadForGetApc &&
       Apc->ApcStateIndex == 0 /*OriginalApcEnvironment*/)
    {
        PsExitSpecialApc = (PVOID)Apc->KernelRoutine;
        PspExitApcRundown = (PVOID)Apc->RundownRoutine;
        PspExitNormalApc = (PVOID)Apc->NormalRoutine;

        //��óɹ���ȡ������
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

//���������Ҫ����ȷ���߳̽���ʱ��APC����
//����ʵ�ֹ�ϵ, ThreadHandle/Thread����Ϊ��Ч�ķ�ϵͳ�߳̾��/ָ��
//Handle  ΪTRUEʱʹ��ThreadHandle
//        ΪFALSEʹ��Thread
BOOLEAN
EnviromentSpecialInitialize(HANDLE ThreadHandle, PVOID Thread, BOOLEAN Handle)
{
    BOOLEAN retVal;
    PETHREAD thread;
    NTSTATUS status;
    KPROCESSOR_MODE previousMode;

    //�Ѿ���ʼ����
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
