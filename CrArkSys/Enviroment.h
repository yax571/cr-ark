#pragma once

//初始化一些偏移量
BOOLEAN
EnviromentInitialize(PDRIVER_OBJECT DriverObject);

//EPROCESS结构中的偏移
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessActiveProcessLinksOffset;
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessObjectTableOffset;
extern ULONG EProcessSectionObjectOffset;
extern ULONG EProcessImageFileNameOffset;
extern ULONG EProcessThreadListHeadOffset;
extern ULONG EProcessObjectTableOffset;

//HANDLE_TABLE中的偏移
extern ULONG HandleTableHandleTableListOffset;
extern ULONG HandleTableQuotaProcessOffset;
extern ULONG HandleTableUniqueProcessIdOffset;

//ETHREAD结构中的偏移
extern ULONG EThreadCidOffset;
extern ULONG EThreadApcStateProcessOffset;
extern ULONG EThreadThreadListEntryOffset;

//动态获得的地址
extern PVOID SystemProcess;
extern PVOID PspCidTable;
extern ULONG NtKernelBase;
extern ULONG NtKernelSize;
extern PLIST_ENTRY ActiveProcessLinksHead;
extern PLIST_ENTRY PsLoadedModuleList;
extern PLIST_ENTRY HandleTableListHead;