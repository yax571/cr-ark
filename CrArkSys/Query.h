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


//根据EProcess结构获得镜像名称和路径
//失败返回NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process);

//这个函数返回LDR_DATA_TABLE_ENTRY列表
//R3程序不能直接使用这个列表
//要配合QueryModuleNameInfo使用
PProcessModuleList
QueryProcessModuleList(PEPROCESS Process);

//根据LDR_DATA_TABLE_ENTRY结构获得模块名称
PModuleNameInfo
QueryModuleNameInfo(PEPROCESS Process,
                    PVOID LdrDataTableEntry);

//查询进程信息
PProcessInfo
QueryProcessInfo(PEPROCESS Process);

//查询线程信息
PThreadInfo
QueryThreadInfo(PETHREAD Thread);