#pragma once

#ifndef CRARKKRNL_EXPORTS
#define CRARKAPI extern "C" __declspec(dllimport)
#else
#define CRARKAPI extern "C" __declspec(dllexport)
#endif

#define OBJECT_ID_TABLE_LENGTH 65535
#define FULL_PATH_LENGTH 266
#define MAX_LDR_DATA_TABLE_LENGTH 65535

typedef struct _ProcessNameInfo {
    char ImageName[17];
    char FullPath[FULL_PATH_LENGTH];
}ProcessNameInfo, *PProcessNameInfo;

typedef struct _ObjectIdPair{
    PVOID Object;
    DWORD UniqueId;
}ObjectIdPair, *PObjectIdPair;

typedef struct _ObjectIdTable{
    WORD Count;
    ObjectIdPair Entry[OBJECT_ID_TABLE_LENGTH];
}ObjectIdTable, *PObjectIdTable;

typedef struct _ProcessInfo {
    char BasePriority;
    unsigned char State;
    LARGE_INTEGER CreateTime;
    DWORD UniqueProcessId;
    DWORD DebugPort;
    DWORD HandleCount;
    DWORD InheritedFromUniqueProcessId;
    PVOID Peb;
}ProcessInfo, *PProcessInfo;

typedef struct _ThreadInfo {
    DWORD Tid;
    PVOID EThread;
    DWORD ContextSwitches;
    char BasePriority;
    PVOID StartAddress;
    BYTE State;
    char ImagePath[FULL_PATH_LENGTH];
}ThreadInfo, *PThreadInfo;

typedef struct _ProcessModuleList {
    PVOID Process;
    WORD Count;
    PVOID LdrDataTable[MAX_LDR_DATA_TABLE_LENGTH];
}ProcessModuleList, *PProcessModuleList;

typedef struct _ModuleInfo {
    DWORD BaseAddress;
    DWORD EntryPoint;
    DWORD SizeOfImage;
    char FullPath[FULL_PATH_LENGTH];
}ModuleInfo, *PModuleInfo;

typedef struct _ServiceTableInfo {
    DWORD Count;
    PVOID OriginAddress[1500];
    PVOID CurrentAddress[1500];
}ServiceTableInfo, *PServiceTableInfo;

//�ͷ������������ɵ���Դ
CRARKAPI VOID WINAPI CrFreeMem(PVOID Address);

//��ʼ��dll�ļ�
//�����ͷš����ء���ʼ����������������
CRARKAPI BOOL WINAPI CrInitialize();

//�ر����ӡ�ж���������ͷ���Դ��
CRARKAPI void WINAPI CrUninitialize();

//ö�ٽ���
CRARKAPI PObjectIdTable WINAPI CrProcessEnum(BOOL ShowDeleting);

//ö���߳�
//ProcessΪCrProcessEnumö�ٳ��Ľ���
CRARKAPI PObjectIdTable WINAPI CrThreadEnum(PVOID Process);

//��ý�������
//ProcessΪCrProcessEnumö�ٳ��Ľ���
CRARKAPI PProcessNameInfo WINAPI CrQueryProcessName(PVOID Process);

//��ȡ������ϸ��Ϣ
//ProcessΪCrProcessEnumö�ٳ��Ľ���
CRARKAPI PProcessInfo WINAPI CrQueryProcessInfo(PVOID Process);

//��ȡ�߳���Ϣ
//ThreadΪCrThreadEnum���ص��߳�ָ��
CRARKAPI PThreadInfo WINAPI CrQueryThreadInfo(PVOID Thread);

//��ȡ���������ص�ģ��
//����ֵ����ֱ��ʹ��
CRARKAPI PProcessModuleList WINAPI CrQueryProcessModuleList(PVOID Process);

//��ȡģ����ϸ��Ϣ
//Process ΪCrQueryProcessModuleList����ֵ�е�Process
//LdrData ΪCrQueryProcessModuleList����ֵ�е�LdrDataTable
CRARKAPI PModuleInfo WINAPI CrQueryModuleInfo(PVOID Process, PVOID LdrData);

//�����߳�
//ThreadΪCrThreadEnum���ص�ָ��
//ExitStatus �˳�����
//ForceExit  �Ƿ�ʹ��ǿ���˳�
CRARKAPI BOOL WINAPI CrTerminateThread(PVOID Thread, DWORD ExitStatus, BOOL ForceExit);

//��������
//ProcessΪCrProcessEnum���ص�ָ��
//ExitStatus �˳�����
//ForceExit  �Ƿ�ʹ��ǿ���˳�
CRARKAPI BOOL WINAPI CrTerminateProcess(PVOID Process, DWORD ExitStatus, BOOL ForceExit);

//ж��Process�����е�ָ��ģ��
//BaseAddress  ģ����ʼ��ַ
CRARKAPI BOOL WINAPI CrUnmapProcessModule(PVOID Process, PVOID BaseAddress);

//��������
//Remove == TRUEʱȡ������
CRARKAPI BOOL WINAPI CrProtectObject(PVOID Object, BOOL Remove);

//�������̺������̲߳�������
//Pid����ID (Pid == 0ʱ�������ý���)
//Protect == TRUE   ��ʼ����
//        == FALSE  ȡ������
CRARKAPI VOID WINAPI CrProtectProcess(DWORD Pid, BOOL Protect);

//���SSDT
CRARKAPI PServiceTableInfo WINAPI CrGetSSDTInfo();

//�޸�SSDT
CRARKAPI BOOL WINAPI CrFixSsdt(DWORD ServiceNumber, LPVOID Address);