#include "stdafx.h"
#include <WinIoCtl.h>
#include "CrArkKrnl.h"
#include "..\CrArkSys\CtrlCode.h"

//枚举进程
PObjectIdTable CrProcessEnum(BOOL ShowDeleting)
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
PObjectIdTable CrThreadEnum(PVOID Process)
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