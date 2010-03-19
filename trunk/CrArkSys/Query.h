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


//根据EProcess结构获得镜像名称和路径
//失败返回NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process);

//这个函数返回LDR_DATA_TABLE_ENTRY列表
//R3程序不能直接使用这个列表
//要配合QueryModuleInfo使用
PProcessModuleList
QueryProcessModuleList(PEPROCESS Process);

//根据LDR_DATA_TABLE_ENTRY结构获得模块名称
PModuleInfo
QueryModuleInfo(PEPROCESS Process,
                PVOID LdrDataTableEntry);

//查询进程信息
PProcessInfo
QueryProcessInfo(PEPROCESS Process);

//查询线程信息
PThreadInfo
QueryThreadInfo(PETHREAD Thread);
