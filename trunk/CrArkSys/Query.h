#pragma once

#include "Undocument.h"

#define FULL_PATH_LENGTH 266
#define MAX_LDR_DATA_TABLE_LENGTH 65535

typedef struct _ProcessNameInfo {
    char ImageName[17];
    char FullPath[FULL_PATH_LENGTH];
}ProcessNameInfo, *PProcessNameInfo;

typedef struct _ProcessModuleList {
    USHORT Count;
    PVOID LdrDataTable[65535];
}ProcessModuleList, *PProcessModuleList;

typedef struct _ModuleNameInfo {
    ULONG Length;
    char FullPath[FULL_PATH_LENGTH];
}ModuleNameInfo, *PModuleNameInfo;

typedef struct _ProcessInfo {
    char BasePriority;
    unsigned char State;
    LARGE_INTEGER CreateTime;
    ULONG UniqueProcessId;
    ULONG DebugPort;
    ULONG HandleCount;
    ULONG InheritedFromUniqueProcessId;
    PVOID Peb;
}ProcessInfo, *PProcessInfo;

typedef struct _ThreadInfo {
    ULONG Tid;
    PVOID EThread;
    ULONG ContextSwitches;
    char BasePriority;
    PVOID StartAddress;
    UCHAR State;
}ThreadInfo, *PThreadInfo;


//����EProcess�ṹ��þ������ƺ�·��
//ʧ�ܷ���NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process);

//�����������LDR_DATA_TABLE_ENTRY�б�
//R3������ֱ��ʹ������б�
//Ҫ���QueryModuleNameInfoʹ��
PProcessModuleList
QueryProcessModuleList(PEPROCESS Process);

//����LDR_DATA_TABLE_ENTRY�ṹ���ģ������
PModuleNameInfo
QueryModuleNameInfo(PEPROCESS Process,
                    PVOID LdrDataTableEntry);

//��ѯ������Ϣ
PProcessInfo
QueryProcessInfo(PEPROCESS Process);

//��ѯ�߳���Ϣ
PThreadInfo
QueryThreadInfo(PETHREAD Thread);