#pragma once

//初始化一些偏移量
BOOLEAN
EnviromentInitialize(PDRIVER_OBJECT DriverObject);

BOOLEAN
EnviromentSpecialInitialize(HANDLE ThreadHandle);

//KRPCESS偏移
extern ULONG KProcessBasePriorityOffset;
extern ULONG KProcessStateOffset;

//EPROCESS结构中的偏移
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessActiveProcessLinksOffset;
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessObjectTableOffset;
extern ULONG EProcessSectionObjectOffset;
extern ULONG EProcessImageFileNameOffset;
extern ULONG EProcessThreadListHeadOffset;
extern ULONG EProcessObjectTableOffset;
extern ULONG EProcessSectionObjectOffset;
extern ULONG EProcessPebOffset;
extern ULONG EProcessCreateTimeOffset;
extern ULONG EProcessDebugPortOffset;
extern ULONG EProcessInheritedFromUniqueProcessIdOffset;

//HANDLE_TABLE中的偏移
extern ULONG HandleTableHandleTableListOffset;
extern ULONG HandleTableQuotaProcessOffset;
extern ULONG HandleTableUniqueProcessIdOffset;
extern ULONG HandleTableHandleCountOffset;

//ETHREAD结构中的偏移
extern ULONG EThreadCidOffset;
extern ULONG EThreadApcStateProcessOffset;
extern ULONG EThreadThreadListEntryOffset;
extern ULONG EThreadStartAddressOffset;
extern ULONG EThreadWin32StartAddressOffset;

//KTHREAD
extern ULONG KThreadContextSwitchesOffset;
extern ULONG KThreadBasePriorityOffset;
extern ULONG KThreadStateOffset;

extern ULONG SectionObjectSegmentOffset;
extern ULONG SegmentObjectSubSecOffset;
extern ULONG ControlAreaFilePointerOffset;

//动态获得的地址
extern PVOID SystemProcess;
extern PVOID PspCidTable;
extern ULONG NtKernelBase;
extern ULONG NtKernelSize;
extern PLIST_ENTRY ActiveProcessLinksHead;
extern PLIST_ENTRY PsLoadedModuleList;
extern PLIST_ENTRY HandleTableListHead;
