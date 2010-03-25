#include "stdafx.h"
#include <WinIoCtl.h>
#include "CrArkKrnl.h"
#include "..\CrArkSys\CtrlCode.h"

//枚举进程
PObjectIdTable WINAPI CrProcessEnum(BOOL ShowDeleting)
{
    PObjectIdTable objIdTable;
    BOOL bRet;
    DWORD dwRet;
    unsigned char deleting = ShowDeleting;

    objIdTable = new ObjectIdTable;
    if(objIdTable == NULL)
        return NULL;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_PROCENUM,
                           &deleting, sizeof(deleting),
                           objIdTable, sizeof(ObjectIdTable),
                           &dwRet, NULL);

    if(!bRet)
    {
        delete objIdTable;
        return NULL;
    }

    return objIdTable;
}

//枚举线程
PObjectIdTable WINAPI CrThreadEnum(PVOID Process)
{
    PObjectIdTable objIdTable;
    BOOL bRet;
    DWORD dwRet;

    objIdTable = new ObjectIdTable;
    if(objIdTable == NULL)
        return NULL;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_THREADENUM,
                           &Process, sizeof(Process),
                           objIdTable, sizeof(ObjectIdTable),
                           &dwRet, NULL);
    if(!bRet)
    {
        delete objIdTable;
        return NULL;
    }

    return objIdTable;
}

//获得进程名称
//Process为CrProcessEnum枚举出的进程
PProcessNameInfo WINAPI CrQueryProcessName(PVOID Process)
{
    PProcessNameInfo processNameInfo;
    BOOL bRet;
    DWORD dwRet;

    processNameInfo = new ProcessNameInfo;
    if(processNameInfo == NULL)
        return NULL;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_QUERYPROCNAME,
                           &Process, sizeof(Process),
                           processNameInfo, sizeof(ProcessNameInfo),
                           &dwRet, NULL);
    if(!bRet)
    {
        delete processNameInfo;
        return NULL;
    }

    return processNameInfo;
}

//获取进程详细信息
//Process为CrProcessEnum枚举出的进程
PProcessInfo WINAPI CrQueryProcessInfo(PVOID Process)
{
    PProcessInfo processInfo;
    BOOL bRet;
    DWORD dwRet;

    processInfo = new ProcessInfo;
    if(processInfo == NULL)
        return NULL;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_QUERYPROCINFO,
                           &Process, sizeof(Process),
                           processInfo, sizeof(ProcessInfo),
                           &dwRet, NULL);
    if(!bRet)
    {
        delete processInfo;
        return NULL;
    }

    return processInfo;
}

//获取线程信息
//Thread为CrThreadEnum返回的线程指针
PThreadInfo WINAPI CrQueryThreadInfo(PVOID Thread)
{
    PThreadInfo threadInfo;
    BOOL bRet;
    DWORD dwRet;

    threadInfo = new ThreadInfo;
    if(threadInfo == NULL)
        return NULL;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_QUERYTHREADINFO,
                           &Thread, sizeof(Thread),
                           threadInfo, sizeof(ThreadInfo),
                           &dwRet, NULL);
    if(!bRet)
    {
        delete threadInfo;
        return NULL;
    }

    return threadInfo;
}

//获取进程所加载的模块
//返回值不能直接使用
PProcessModuleList WINAPI CrQueryProcessModuleList(PVOID Process)
{
    PProcessModuleList procModList;
    BOOL bRet;
    DWORD dwRet;

    procModList = new ProcessModuleList;
    if(procModList == NULL)
        return NULL;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_QUERYPROCMODULE,
                           &Process, sizeof(Process),
                           procModList, sizeof(ProcessModuleList),
                           &dwRet, NULL);
    if(!bRet)
    {
        delete procModList;
        return NULL;
    }

    return procModList;
}

//获取模块详细信息
//Process 为CrQueryProcessModuleList返回值中的Process
//LdrData 为CrQueryProcessModuleList返回值中的LdrDataTable
PModuleInfo WINAPI CrQueryModuleInfo(PVOID Process, PVOID LdrData)
{
    PModuleInfo modInfo;
    BOOL bRet;
    DWORD dwRet;
    PVOID input[2];

    modInfo = new ModuleInfo;
    if(modInfo == NULL)
        return NULL;

    input[0] = Process;
    input[1] = LdrData;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_QUERYMODULEINFO,
                           input, sizeof(input),
                           modInfo, sizeof(ModuleInfo),
                           &dwRet, NULL);
    if(!bRet)
    {
        delete modInfo;
        return NULL;
    }

    return modInfo;
}

//结束线程
//Thread为CrThreadEnum返回的指针
//ExitStatus 退出参数
//ForceExit  是否使用强制退出
BOOL WINAPI CrTerminateThread(PVOID Thread, DWORD ExitStatus, BOOL ForceExit)
{
    DWORD input[3];
    BOOL bRet;
    DWORD dwRet;

    input[0] = (DWORD)Thread;
    input[1] = (DWORD)ExitStatus;
    input[2] = (DWORD)ForceExit;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_TERMINATETHREAD,
                           input, sizeof(input),
                           NULL, 0,
                           &dwRet, NULL);

    return bRet;
}

//结束进程
//Process为CrProcessEnum返回的指针
//ExitStatus 退出参数
//ForceExit  是否使用强制退出
BOOL WINAPI CrTerminateProcess(PVOID Process, DWORD ExitStatus, BOOL ForceExit)
{
    DWORD input[3];
    BOOL bRet;
    DWORD dwRet;

    input[0] = (DWORD)Process;
    input[1] = (DWORD)ExitStatus;
    input[2] = (DWORD)ForceExit;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_TERMINATEPROC,
                           input, sizeof(input),
                           NULL, 0,
                           &dwRet, NULL);

    return bRet;
}

//卸载Process进程中的指定模块
//BaseAddress  模块起始地址
BOOL WINAPI CrUnmapProcessModule(PVOID Process, PVOID BaseAddress)
{
    DWORD input[2];
    BOOL bRet;
    DWORD dwRet;

    input[0] = (DWORD)Process;
    input[1] = (DWORD)BaseAddress;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_UNMAPMODULE,
                           input, sizeof(input),
                           NULL, 0,
                           &dwRet, NULL);

    return bRet;
}

BOOL WINAPI CrProtectObject(PVOID Object, BOOL Remove)
{
    DWORD input[2];
    BOOL bRet;
    DWORD dwRet;

    input[0] = (DWORD)Object;
    input[1] = (DWORD)Remove;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_PROTECT,
                           input, sizeof(input),
                           NULL, 0,
                           &dwRet, NULL);

    return bRet;
}