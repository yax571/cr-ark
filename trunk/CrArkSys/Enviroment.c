#include <ntddk.h>
#include "Enviroment.h"
#include "ade32.h"
#include "Undocument.h"

//EPROCESS结构中的偏移
ULONG EProcessUniqueProcessIdOffset;
ULONG EProcessActiveProcessLinksOffset;
ULONG EProcessUniqueProcessIdOffset;
ULONG EProcessObjectTableOffset;
ULONG EProcessSectionObjectOffset;
ULONG EProcessImageFileNameOffset;
ULONG EProcessThreadListHeadOffset;
ULONG EProcessObjectTableOffset;

ULONG HandleTableHandleTableListOffset;
ULONG HandleTableQuotaProcessOffset;
ULONG HandleTableUniqueProcessIdOffset;

ULONG EThreadCidOffset;
ULONG EThreadApcStateProcessOffset;
ULONG EThreadThreadListEntryOffset;

//动态获得的地址
PVOID SystemProcess;
PVOID PspCidTable;
ULONG NtKernelBase;
ULONG NtKernelSize;
PLIST_ENTRY ActiveProcessLinksHead;
PLIST_ENTRY PsLoadedModuleList;
PLIST_ENTRY HandleTableListHead;


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
        EProcessUniqueProcessIdOffset = 0x84;
        EProcessObjectTableOffset = 0x0c4;
        EProcessSectionObjectOffset = 0x138;
        EProcessImageFileNameOffset = 0x174;
        EProcessThreadListHeadOffset = 0x190;
        EProcessObjectTableOffset = 0x0c4; 
        EThreadCidOffset = 0x1ec;
        EThreadApcStateProcessOffset = 0x044;
        EThreadThreadListEntryOffset = 0x22c;
        HandleTableHandleTableListOffset = 0x01c;
        HandleTableQuotaProcessOffset = 0x004;
        HandleTableUniqueProcessIdOffset = 0x08;
        break;
    }
    
    //获得System进程
    SystemProcess = PsGetCurrentProcess();
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