#include "stdafx.h"
#include <WinIoCtl.h>
#include "CrArkKrnl.h"
#include "..\CrArkSys\CtrlCode.h"

//ö�ٽ���
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

//ö���߳�
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

//��ý�������
//ProcessΪCrProcessEnumö�ٳ��Ľ���
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

//��ȡ������ϸ��Ϣ
//ProcessΪCrProcessEnumö�ٳ��Ľ���
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

//��ȡ�߳���Ϣ
//ThreadΪCrThreadEnum���ص��߳�ָ��
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

//��ȡ���������ص�ģ��
//����ֵ����ֱ��ʹ��
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

//��ȡģ����ϸ��Ϣ
//Process ΪCrQueryProcessModuleList����ֵ�е�Process
//LdrData ΪCrQueryProcessModuleList����ֵ�е�LdrDataTable
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