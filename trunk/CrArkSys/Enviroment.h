#pragma once

//��ʼ��һЩƫ����
BOOLEAN
EnviromentInitialize(PDRIVER_OBJECT DriverObject);

//���������Ҫ����ȷ���߳̽���ʱ��APC����
//����ʵ�ֹ�ϵ, ThreadHandle/Thread����Ϊ��Ч�ķ�ϵͳ�߳̾��/ָ��
//Handle  ΪTRUEʱʹ��ThreadHandle
//        ΪFALSEʹ��Thread
BOOLEAN
EnviromentSpecialInitialize(HANDLE ThreadHandle, PVOID Thread, BOOLEAN Handle);

//KRPCESSƫ��
extern ULONG KProcessBasePriorityOffset;
extern ULONG KProcessStateOffset;

//EPROCESS�ṹ�е�ƫ��
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

//HANDLE_TABLE�е�ƫ��
extern ULONG HandleTableHandleTableListOffset;
extern ULONG HandleTableQuotaProcessOffset;
extern ULONG HandleTableUniqueProcessIdOffset;
extern ULONG HandleTableHandleCountOffset;

//ETHREAD�ṹ�е�ƫ��
extern ULONG EThreadCidOffset;
extern ULONG EThreadApcStateProcessOffset;
extern ULONG EThreadThreadListEntryOffset;
extern ULONG EThreadStartAddressOffset;
extern ULONG EThreadWin32StartAddressOffset;

//KTHREAD
extern ULONG KThreadContextSwitchesOffset;
extern ULONG KThreadBasePriorityOffset;
extern ULONG KThreadStateOffset;
extern ULONG KThreadPreviousModeOffset;

extern ULONG SectionObjectSegmentOffset;
extern ULONG SegmentObjectSubSecOffset;
extern ULONG ControlAreaFilePointerOffset;

//��̬��õĵ�ַ
extern PVOID SystemProcess;
extern PVOID PspCidTable;
extern ULONG NtKernelBase;
extern ULONG NtKernelSize;
extern PLIST_ENTRY ActiveProcessLinksHead;
extern PLIST_ENTRY PsLoadedModuleList;
extern PLIST_ENTRY HandleTableListHead;

extern UCHAR KeInsertQueueApcJumpBack[20];


//������ֹ�̵߳�APC-Routine
extern PVOID PsExitSpecialApc;
extern PVOID PspExitApcRundown;
extern PVOID PspExitNormalApc;

//��̬��õĺ��� 
NTSTATUS 
(__stdcall *NtTerminateThread) (
                    __in_opt HANDLE ThreadHandle,
                    __in NTSTATUS ExitStatus
                     );