#include "stdafx.h"
#include "CrArkKrnl.h"

//释放驱动到系统目录
//若成功, Path返回驱动路径
BOOL ReleaseDriver(TCHAR* Path)
{
    TCHAR szCurrentDir[MAX_PATH];
    TCHAR szSysDir[MAX_PATH];
    TCHAR szFileDir[MAX_PATH];
    UINT ret;
    BOOL bRet;

    GetCurrentDirectory(MAX_PATH, szCurrentDir);
    lstrcat(szCurrentDir, _T("\\CrArkSys.sys"));
    
    GetSystemDirectory(szSysDir, MAX_PATH);
    ret = GetTempFileName(szSysDir, _T("cr"), 0, szFileDir);

    if(ret == 0)
        return FALSE;

    bRet= CopyFile(szCurrentDir, szFileDir, FALSE);
    if(bRet == FALSE)
        return FALSE;

    lstrcpy(Path, szFileDir);
    return TRUE;
}

BOOL LoadDriver()
{
    BOOL retVal = FALSE;
    TCHAR path[MAX_PATH];

    //首先释放驱动
    retVal = ReleaseDriver(path);
    if(!retVal)
        return retVal;

    
    return retVal;
}

BOOL CrInitialize()
{
    BOOL retVal = FALSE;

    if(!LoadDriver())
        return FALSE;


    return retVal;
}