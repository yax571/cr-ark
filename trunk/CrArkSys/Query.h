#pragma once

#include "Undocument.h"

#define FULL_PATH_LENGTH 266
#define MAX_LDR_DATA_TABLE_LENGTH 65535

typedef struct _ProcessNameInfo {
    char ImageName[17];
    char FullPath[FULL_PATH_LENGTH];
}ProcessNameInfo, *PProcessNameInfo;

typedef struct _ProcessModuleList {
    PVOID Process;
    USHORT Count;
    PVOID LdrDataTable[MAX_LDR_DATA_TABLE_LENGTH];
}ProcessModuleList, *PProcessModuleList;

typedef struct _ModuleInfo {
    ULONG BaseAddress;
    ULONG EntryPoint;
    ULONG SizeOfImage;
    char FullPath[FULL_PATH_LENGTH];
}ModuleInfo, *PModuleInfo;

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
    char ImagePath[FULL_PATH_LENGTH];
}ThreadInfo, *PThreadInfo;


//����EProcess�ṹ��þ������ƺ�·��
//ʧ�ܷ���NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process);

//�����������LDR_DATA_TABLE_ENTRY�б�
//R3������ֱ��ʹ������б�
//Ҫ���QueryModuleInfoʹ��
PProcessModuleList
QueryProcessModuleList(PEPROCESS Process);

//����LDR_DATA_TABLE_ENTRY�ṹ���ģ������
PModuleInfo
QueryModuleInfo(PEPROCESS Process,
                PVOID LdrDataTableEntry);

//��ѯ������Ϣ
PProcessInfo
QueryProcessInfo(PEPROCESS Process);

//��ѯ�߳���Ϣ
PThreadInfo
QueryThreadInfo(PETHREAD Thread);
