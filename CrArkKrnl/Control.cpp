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