#pragma once

//��ʼ��һЩƫ����
BOOLEAN
EnviromentInitialize(PDRIVER_OBJECT DriverObject);

//EPROCESS�ṹ�е�ƫ��
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessActiveProcessLinksOffset;
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessObjectTableOffset;
extern ULONG EProcessSectionObjectOffset;
extern ULONG EProcessImageFileNameOffset;
extern ULONG EProcessThreadListHeadOffset;
extern ULONG EProcessObjectTableOffset;

//HANDLE_TABLE�е�ƫ��
extern ULONG HandleTableHandleTableListOffset;
extern ULONG HandleTableQuotaProcessOffset;
extern ULONG HandleTableUniqueProcessIdOffset;

//ETHREAD�ṹ�е�ƫ��
extern ULONG EThreadCidOffset;
extern ULONG EThreadApcStateProcessOffset;
extern ULONG EThreadThreadListEntryOffset;

//��̬��õĵ�ַ
extern PVOID SystemProcess;
extern PVOID PspCidTable;
extern ULONG NtKernelBase;
extern ULONG NtKernelSize;
extern PLIST_ENTRY ActiveProcessLinksHead;
extern PLIST_ENTRY PsLoadedModuleList;
extern PLIST_ENTRY HandleTableListHead;