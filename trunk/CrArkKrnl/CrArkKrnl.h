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