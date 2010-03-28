#include <ntifs.h>
#include "Undocument.h"
#include "Enviroment.h"
#include "ProcEnum.h"
#include "helper.h"
#include "Query.h"

//EPROCESS->Peb->Ldr->InLoadOrderModuleList.FLink指向
//_LDR_DATA_TABLE_ENTRY结构  _LDR_DATA_TABLE_ENTRY->FullDllName就是Exe文件全路径
BOOLEAN
QueryProcessFullPathByPeb(PProcessNameInfo ProcNameInfo,
                          PEPROCESS Process)
{
    BOOLEAN ret;
    BOOLEAN attached;
    KAPC_STATE oldApcState;
    PUCHAR magic;
    ANSI_STRING ansiString;
    PUNICODE_STRING unicodeString;
    NTSTATUS status;

    ret = FALSE;
    attached = FALSE;
    magic = (PUCHAR)Process;

    //如果不是当前进程, 先进入目标进程的地址空间
    if(Process != PsGetCurrentProcess())
    {
        KeStackAttachProcess(Process, &oldApcState);
        attached = TRUE;
    }

    //用户态空间处理
    try {
        //EPROCESS->Peb
        magic = *(PUCHAR*)(magic + EProcessPebOffset);
        
        //Peb->Ldr
        ProbeForRead(magic + 0x0c, 4, 1);
        magic = *(PUCHAR*)(magic + 0x0c);

        //Ldr->InLoadOrderModuleList.FLink
        ProbeForRead(magic + 0x0c, 4, 1);
        magic = *(PUCHAR*)(magic + 0x0c);

        //_LDR_DATA_TABLE_ENTRY.FullDllName
        ProbeForRead(magic + 0x24, sizeof(UNICODE_STRING), 1);
        magic = magic + 0x24;

        //复制
        unicodeString = (PUNICODE_STRING)magic;
        RtlZeroMemory(ProcNameInfo->FullPath, FULL_PATH_LENGTH);
        ansiString.Buffer = ProcNameInfo->FullPath;
        ansiString.Length = 0;
        ansiString.MaximumLength = FULL_PATH_LENGTH;

        status = RtlUnicodeStringToAnsiString(&ansiString, 
                                              unicodeString, 
                                              FALSE);
        if(NT_SUCCESS(status))
            ret = TRUE;
    } except(EXCEPTION_EXECUTE_HANDLER) {
        ret = FALSE;
    }

    if(attached)
    {
        KeUnstackDetachProcess(&oldApcState);
    }

    return ret;
}


//EPROCESS->ImageFileName镜像名称
//EPROCESS->SectionObject->Segment->SubSection->ControlArea->FilePointer是一个FileObject
//用这个文件对象调用ObQueryNameString可以获得全路径
//也可自己构建IRP下发查询
BOOLEAN
QueryProcessFullPathBySection(PProcessNameInfo ProcNameInfo,
                              PEPROCESS Process)
{
    PUCHAR magic;
    PFILE_OBJECT filePointer;
    POBJECT_NAME_INFORMATION objectNameInfo;
    UNICODE_STRING dosFullPath;
    NTSTATUS status;
    ULONG retLength;
    ANSI_STRING fullPath;

    magic = (PUCHAR)Process;

    //取得FileObject指针
    //magic = EPROCESS.SectionObject
    if(MmIsAddressValid(magic + EProcessSectionObjectOffset) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic + EProcessSectionObjectOffset);

    //magic = SectionObject.Segment
    if(MmIsAddressValid(magic + SectionObjectSegmentOffset) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic + SectionObjectSegmentOffset);

    //magic = SegmentObject.SubSection
    if(MmIsAddressValid(magic + SegmentObjectSubSecOffset) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic + SegmentObjectSubSecOffset);

    //magic = SubSection.ControlArea
    if(MmIsAddressValid(magic) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic);

    //ControlArea.FilePointer
    if(MmIsAddressValid(magic + ControlAreaFilePointerOffset) == FALSE)
        return FALSE;
    filePointer = *(PFILE_OBJECT*)(magic + ControlAreaFilePointerOffset);

    //增加引用计数, 防删
    status = ObReferenceObjectByPointer(filePointer,
                                        0,
                                        *IoFileObjectType,
                                        KernelMode);
    if(!NT_SUCCESS(status))
        return FALSE;

    //终于得到了FileObject指针.....
    objectNameInfo = ExAllocatePoolWithTag(PagedPool,
                                           1024,
                                           'obni');
    if(objectNameInfo == NULL)
        return FALSE;
    status = ObQueryNameString(filePointer,
                               objectNameInfo,
                               1024,
                               &retLength);
    if(!NT_SUCCESS(status) || retLength > 1024)
    {
        ExFreePool(objectNameInfo);
        ObDereferenceObject(filePointer);
        return FALSE;
    }

    fullPath.Buffer = ProcNameInfo->FullPath;
    fullPath.Length = 0;
    fullPath.MaximumLength = FULL_PATH_LENGTH;
    RtlZeroMemory(ProcNameInfo->FullPath, FULL_PATH_LENGTH);

    //unicode转换为ansi
    if(ConvertDevicePathToDosPath(&objectNameInfo->Name,
                                  TRUE,
                                  PagedPool,
                                  &dosFullPath))
    {
        status = RtlUnicodeStringToAnsiString(&fullPath,
                                              &dosFullPath,
                                              FALSE);
        ExFreePool(dosFullPath.Buffer);
    }
    else
    {
        status = RtlUnicodeStringToAnsiString(&fullPath,
                                              &objectNameInfo->Name,
                                              FALSE);
    }
    if(!NT_SUCCESS(status))
    {
        ExFreePool(objectNameInfo);
        ObDereferenceObject(filePointer);
        return FALSE;
    }

    ExFreePool(objectNameInfo);
    ObDereferenceObject(filePointer);
    return TRUE;
}


//根据EProcess结构获得镜像名称和路径
//失败返回NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process)
{
    PProcessNameInfo procNameInfo;

    if(!IsProcessObject(Process))
    {
        KdPrint(("QueryProcessName: Param Not A EPROCESS\n"));
        return NULL;
    }

    procNameInfo = ExAllocatePoolWithTag(PagedPool,
                                         sizeof(ProcessNameInfo),
                                         'pnin');
    if(procNameInfo == NULL)
    {
        KdPrint(("allocate ProcessNameInfo fail.\n"));
        return NULL;
    }

    //系统进程和正在退出的进程特殊处理
    if(Process == SystemProcess)
    {
        RtlCopyMemory(procNameInfo->ImageName, "[System]", 8);
        procNameInfo->ImageName[8] = 0;
        RtlCopyMemory(procNameInfo->FullPath, "NT System Kernel", 16);
        procNameInfo->FullPath[16] = 0;

        return procNameInfo;
    }
    else if(IsProcessDeleting(Process))
    {
        RtlCopyMemory(procNameInfo->ImageName, "Deleting", 8);
        procNameInfo->ImageName[8] = 0;
        RtlCopyMemory(procNameInfo->FullPath, "Deleting", 8);
        procNameInfo->FullPath[8] = 0;

        return procNameInfo;
    }

    //ERPCESS.ImageFileName获得镜像名称
    if(MmIsAddressValid((PUCHAR)Process + EProcessImageFileNameOffset) == FALSE)
    {
        ExFreePool(procNameInfo);
        return NULL;
    }
    RtlCopyMemory(procNameInfo->ImageName, 
                  (PUCHAR)Process + EProcessImageFileNameOffset,
                  16);
    procNameInfo->ImageName[16] = 0;

    //获得文件全路径
    if(QueryProcessFullPathBySection(procNameInfo, Process))
        return procNameInfo;

    if(QueryProcessFullPathByPeb(procNameInfo, Process))
        return procNameInfo;

    ExFreePool(procNameInfo);
    return NULL;
}


//这个函数返回LDR_DATA_TABLE_ENTRY列表
//R3程序不能直接使用这个列表
PProcessModuleList
QueryProcessModuleList(PEPROCESS Process)
{
    PLIST_ENTRY current, head;
    PUCHAR magic;
    BOOLEAN attached;
    KAPC_STATE apcState;
    PProcessModuleList processModuleList;
    BOOLEAN success;
    KIRQL oldIrql;

    if(!IsProcessObject(Process) ||
        IsProcessDeleting(Process))
    {
        KdPrint(("Process object not valid.\n"));
        return NULL;
    }

    processModuleList = ExAllocatePoolWithTag(PagedPool,
                                              sizeof(ProcessModuleList),
                                              'pmls');
    RtlZeroMemory(processModuleList, sizeof(ProcessModuleList));
    processModuleList->Process = Process;

    //system进程特殊处理
    if(Process == SystemProcess)
    {
        oldIrql = KeRaiseIrqlToDpcLevel();

        head = PsLoadedModuleList;
        current = PsLoadedModuleList->Flink;

        while(current != head)
        {
            processModuleList->LdrDataTable[processModuleList->Count] = 
                CONTAINING_RECORD(current, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
            processModuleList->Count++;
            current = current->Flink;
        }

        KeLowerIrql(oldIrql);
        return processModuleList;
    }

    //普通进程的处理
    attached = FALSE;
    success = FALSE;
    if(Process != PsGetCurrentProcess())
    {
        KeStackAttachProcess(Process, &apcState);
        attached = TRUE;
    }

    try {
        //magic = EPROCESS->Peb
        magic = *(PUCHAR*)((ULONG)Process + EProcessPebOffset);
        //magic = Peb->Ldr
        ProbeForRead(magic + 0x0c, 4, 4);
        magic = *(PUCHAR*)(magic + 0x0c);
        //magic = Ldr.InLoadOrderModuleList
        magic = magic + 0x0c;

        ProbeForRead(magic, sizeof(LIST_ENTRY), 4);
        head = (PLIST_ENTRY)magic;
        current = head->Flink;

        while(current != head)
        {
            processModuleList->LdrDataTable[processModuleList->Count] = 
                CONTAINING_RECORD(current, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
            processModuleList->Count++;
            current = current->Flink;
        }

        success = TRUE;
    } except(EXCEPTION_EXECUTE_HANDLER) {
        success = FALSE;
    }

    //清理
    if(attached)
        KeUnstackDetachProcess(&apcState);

    if(!success)
    {
        ExFreePool(processModuleList);
        return NULL;
    }

    return processModuleList;
}

//根据LDR_DATA_TABLE_ENTRY结构获得模块名称
PModuleInfo
QueryModuleInfo(PEPROCESS Process,
                PVOID LdrDataTableEntry)
{
    PLDR_DATA_TABLE_ENTRY entry;
    BOOLEAN attached;
    KAPC_STATE apcState;
    PModuleInfo moduleInfo = NULL;
    ANSI_STRING ansiString;
    NTSTATUS status;

    entry = (PLDR_DATA_TABLE_ENTRY)LdrDataTableEntry;

    //验证
    if(!IsProcessObject(Process) ||
        IsProcessDeleting(Process))
        return NULL;

    //申请空间
    moduleInfo = ExAllocatePoolWithTag(PagedPool,
                                       sizeof(ModuleInfo),
                                       'mnin');
    if(!moduleInfo)
        return NULL;
    
    //是否在目标地址空间
    attached = FALSE;
    if(Process != SystemProcess &&
       Process != PsGetCurrentProcess())
    {
        KeStackAttachProcess(Process, &apcState);
        attached = TRUE;
    }

    //从用户空间复制模块信息
    try {
        ansiString.Buffer = moduleInfo->FullPath;
        ansiString.Length = 0;
        ansiString.MaximumLength = FULL_PATH_LENGTH;
        status = RtlUnicodeStringToAnsiString(&ansiString,
                                              &entry->FullDllName,
                                              FALSE);
        if(NT_SUCCESS(status))
            moduleInfo->FullPath[ansiString.Length] = 0;

        moduleInfo->BaseAddress = (ULONG)entry->DllBase;
        moduleInfo->EntryPoint = (ULONG)entry->EntryPoint;
        moduleInfo->SizeOfImage = entry->SizeOfImage;
    }except(EXCEPTION_EXECUTE_HANDLER){
        ExFreePool(moduleInfo);
        moduleInfo = NULL;
    }

    if(attached)
        KeUnstackDetachProcess(&apcState);
    return moduleInfo;
}


//查询进程信息
PProcessInfo
QueryProcessInfo(PEPROCESS Process)
{
    PProcessInfo processInfo;
    PUCHAR magic;
    LARGE_INTEGER sysTime;

    if(!IsProcessObject(Process) ||
        IsProcessDeleting(Process))
        return NULL;

    processInfo = ExAllocatePoolWithTag(PagedPool,
                                        sizeof(ProcessInfo),
                                        'prin');
    if(!processInfo)
        return NULL;

    RtlZeroMemory(processInfo, sizeof(ProcessInfo));

    magic = (PUCHAR)Process;
    processInfo->BasePriority = *(PCHAR)(magic + KProcessBasePriorityOffset);
    sysTime = *(PLARGE_INTEGER)(magic + EProcessCreateTimeOffset);
    ExSystemTimeToLocalTime(&sysTime, &processInfo->CreateTime);
    processInfo->DebugPort = *(PULONG)(magic + EProcessDebugPortOffset);
    processInfo->InheritedFromUniqueProcessId = *(PULONG)(magic + EProcessInheritedFromUniqueProcessIdOffset);
    processInfo->Peb = *(PVOID*)(magic + EProcessPebOffset);
    processInfo->State = *(PUCHAR)(magic + KProcessStateOffset);
    processInfo->UniqueProcessId = *(PULONG)(magic + EProcessUniqueProcessIdOffset);
    
    //Handle_TABLE
    magic = *(PUCHAR*)(magic + EProcessObjectTableOffset);
    processInfo->HandleCount = *(PULONG)(magic + HandleTableHandleCountOffset);

    return processInfo;
}

//查询线程信息
PThreadInfo
QueryThreadInfo(PETHREAD Thread)
{
    PEPROCESS process;
    PUCHAR magic;
    PThreadInfo threadInfo;
    PVOID startAddress;
    PProcessModuleList processModList = NULL;
    BOOLEAN attached = FALSE;
    KAPC_STATE apcState;
    PLDR_DATA_TABLE_ENTRY entry;
    ULONG i;
    ANSI_STRING ansiString;
    NTSTATUS status;

    //常规检查
    if(!IsThreadObject(Thread))
        return NULL;

    process = IoThreadToProcess(Thread);
    if(IsProcessDeleting(process))
        return NULL;

    threadInfo = ExAllocatePoolWithTag(PagedPool,
                                       sizeof(ThreadInfo),
                                       'thin');
    if(!threadInfo)
        return NULL;

    RtlZeroMemory(threadInfo, sizeof(ThreadInfo));
    magic = (PUCHAR)Thread;

    threadInfo->BasePriority = *(PCHAR)(magic + KThreadBasePriorityOffset);
    threadInfo->ContextSwitches = *(PULONG)(magic + KThreadContextSwitchesOffset);
    threadInfo->EThread = magic;
    threadInfo->State = *(PUCHAR)(magic + KThreadStateOffset);
    threadInfo->Tid = *(PULONG)(magic + EThreadCidOffset + sizeof(ULONG));

    startAddress = *(PVOID*)(magic + EThreadWin32StartAddressOffset);
    if(startAddress == NULL)
        startAddress = *(PVOID*)(magic + EThreadStartAddressOffset);
    threadInfo->StartAddress = startAddress;

    //获得模块名称
    //先写个默认名称
    RtlCopyMemory(threadInfo->ImagePath, "(unknown)", 9);
    threadInfo->ImagePath[9] = 0;
    
    //首先遍历LDR_DATA_TABLE_ENTRY获得线程所在模块
    processModList = QueryProcessModuleList(process);
    if(!processModList)
        goto _QueryThreadInfoExit;
    
    //决定是否挂靠
    attached = FALSE;
    if(process != SystemProcess &&
        process != PsGetCurrentProcess())
    {
        KeStackAttachProcess(process, &apcState);
        attached = TRUE;
    }

    //比较起始地址在哪个模块中
    for(i = 0; i < processModList->Count; i++)
    {
        entry = (PLDR_DATA_TABLE_ENTRY)processModList->LdrDataTable[i];
        try {
            //得到地址了
            if(startAddress >= entry->DllBase &&
                startAddress < (PVOID)((PUCHAR)entry->DllBase + entry->SizeOfImage))
            {
                ansiString.Buffer = threadInfo->ImagePath;
                ansiString.Length = 0;
                ansiString.MaximumLength = FULL_PATH_LENGTH;
                status = RtlUnicodeStringToAnsiString(&ansiString,
                                                      &entry->FullDllName,
                                                      FALSE);
                if(NT_SUCCESS(status))
                    ansiString.Buffer[ansiString.Length] = 0;
                break;
            }
        }except(EXCEPTION_EXECUTE_HANDLER) {

        }
    }

_QueryThreadInfoExit:
    if(attached)
        KeUnstackDetachProcess(&apcState);

    if(processModList)
        ExFreePool(processModList);
    return threadInfo;
}