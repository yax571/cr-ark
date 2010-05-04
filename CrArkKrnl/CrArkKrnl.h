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

//释放其它函数生成的资源
CRARKAPI VOID WINAPI CrFreeMem(PVOID Address);

//初始化dll文件
//包括释放、加载、初始化、链接驱动程序
CRARKAPI BOOL WINAPI CrInitialize();

//关闭连接、卸载驱动、释放资源等
CRARKAPI void WINAPI CrUninitialize();

//枚举进程
CRARKAPI PObjectIdTable WINAPI CrProcessEnum(BOOL ShowDeleting);

//枚举线程
//Process为CrProcessEnum枚举出的进程
CRARKAPI PObjectIdTable WINAPI CrThreadEnum(PVOID Process);

//获得进程名称
//Process为CrProcessEnum枚举出的进程
CRARKAPI PProcessNameInfo WINAPI CrQueryProcessName(PVOID Process);

//获取进程详细信息
//Process为CrProcessEnum枚举出的进程
CRARKAPI PProcessInfo WINAPI CrQueryProcessInfo(PVOID Process);

//获取线程信息
//Thread为CrThreadEnum返回的线程指针
CRARKAPI PThreadInfo WINAPI CrQueryThreadInfo(PVOID Thread);

//获取进程所加载的模块
//返回值不能直接使用
CRARKAPI PProcessModuleList WINAPI CrQueryProcessModuleList(PVOID Process);

//获取模块详细信息
//Process 为CrQueryProcessModuleList返回值中的Process
//LdrData 为CrQueryProcessModuleList返回值中的LdrDataTable
CRARKAPI PModuleInfo WINAPI CrQueryModuleInfo(PVOID Process, PVOID LdrData);

//结束线程
//Thread为CrThreadEnum返回的指针
//ExitStatus 退出参数
//ForceExit  是否使用强制退出
CRARKAPI BOOL WINAPI CrTerminateThread(PVOID Thread, DWORD ExitStatus, BOOL ForceExit);

//结束进程
//Process为CrProcessEnum返回的指针
//ExitStatus 退出参数
//ForceExit  是否使用强制退出
CRARKAPI BOOL WINAPI CrTerminateProcess(PVOID Process, DWORD ExitStatus, BOOL ForceExit);

//卸载Process进程中的指定模块
//BaseAddress  模块起始地址
CRARKAPI BOOL WINAPI CrUnmapProcessModule(PVOID Process, PVOID BaseAddress);

//保护对象
//Remove == TRUE时取消保护
CRARKAPI BOOL WINAPI CrProtectObject(PVOID Object, BOOL Remove);

//保护进程和所属线程不被结束
//Pid进程ID (Pid == 0时保护调用进程)
//Protect == TRUE   开始保护
//        == FALSE  取消保护
CRARKAPI VOID WINAPI CrProtectProcess(DWORD Pid, BOOL Protect);

//检测SSDT
CRARKAPI PServiceTableInfo WINAPI CrGetSSDTInfo();

//修复SSDT
CRARKAPI BOOL WINAPI CrFixSsdt(DWORD ServiceNumber, LPVOID Address);