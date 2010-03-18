#include "stdafx.h"
#include "CrArkKrnl.h"
#include "ntapi.h"
#include "shlwapi.h"
#include "winioctl.h"
#include "..\CrArkSys\CtrlCode.h"

#define SERVICENAME (_T("CrArk"))
#define SERVICEPATH (_T("System\\CurrentControlSet\\Services\\CrArk"))
#define SERVICENAME_UNICODE (L"CrArk")
#define DEVICENAME (_T("\\\\.\\CRARKSYS"))

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

    lstrcpy(Path, _T("\\??\\"));
    lstrcat(Path, szFileDir);
    return TRUE;
}

//在注册表System\CurrentControlSet\Services键下注册一个服务
//用来给ZwLoadDriver和ZwUnloadDriver使用
BOOL MakeRegistryEntry(TCHAR *Path)
{
    BOOL retVal = FALSE;
    HKEY hKey = NULL;
    DWORD value1, value0;

    if(RegOpenKey(HKEY_LOCAL_MACHINE, SERVICEPATH, &hKey) != ERROR_SUCCESS &&
       RegCreateKey(HKEY_LOCAL_MACHINE, SERVICEPATH, &hKey) != ERROR_SUCCESS)
    {
        //不能打开, 不能创建, 失败
        goto _MakeRegistryEntryExit;
    }
    
    value0 = 0;
    value1 = 1;
    if(RegSetValueEx(hKey, _T("DisplayName"), 0, 
                     REG_SZ, (const PBYTE)SERVICENAME, sizeof(SERVICENAME)) != ERROR_SUCCESS ||
       RegSetValueEx(hKey, _T("ErrorControl"), 0,
                     REG_DWORD, (const PBYTE)&value1, sizeof(value1)) != ERROR_SUCCESS ||
       RegSetValueEx(hKey, _T("ImagePath"), 0,
                     REG_SZ, (const BYTE*)Path, (lstrlen(Path) + 1) * sizeof(TCHAR)) != ERROR_SUCCESS ||
       RegSetValueEx(hKey, _T("Type"), 0,
                     REG_DWORD, (const PBYTE)&value1, sizeof(value1)) != ERROR_SUCCESS ||
       RegSetValueEx(hKey, _T("Start"), 0,
                     REG_DWORD, (const PBYTE)&value0, sizeof(value0)) != ERROR_SUCCESS 
       )
    {
        goto _MakeRegistryEntryExit;
    }

    retVal = TRUE;

_MakeRegistryEntryExit:
    if(hKey != NULL)
    {
        if(retVal == FALSE)
            RegDeleteKey(HKEY_LOCAL_MACHINE, SERVICEPATH);
        RegCloseKey(hKey);
    }
    
    return retVal;
}

//加载驱动
BOOL LoadDriver()
{
    BOOL retVal = FALSE;
    TCHAR path[MAX_PATH];
    WCHAR registryPath[MAX_PATH];
    UNICODE_STRING uniDriverName;
    NTSTATUS status;

    //首先释放驱动
    retVal = ReleaseDriver(path);
    if(!retVal)
        return retVal;

    //填写注册表项
    retVal = MakeRegistryEntry(path);
    if(!retVal)
    {
        DeleteFile(path);
        return retVal;
    }
    
    //加载驱动
    wsprintfW(registryPath, L"%s\\%s", 
              L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\",
              SERVICENAME_UNICODE);
    RtlInitUnicodeString(&uniDriverName,
                         registryPath);
    status = ZwLoadDriver(&uniDriverName);
    
    //加载完毕可以清理注册表项和文件了
    SHDeleteKey(HKEY_LOCAL_MACHINE, SERVICEPATH);
    DeleteFile(&path[4]);

    retVal = NT_SUCCESS(status);
    if(retVal)
    {
        lstrcpy(DriverPath, path);
    }

    return retVal;
}

//卸载驱动
void UnloadDriver()
{
    BOOL retVal;
    WCHAR registryEntry[MAX_PATH];
    UNICODE_STRING uniRegistryEntry;

    retVal = MakeRegistryEntry(DriverPath);
    if(!retVal)
        return;

    wsprintfW(registryEntry, L"%s\\%s", 
              L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\",
              SERVICENAME_UNICODE);
    RtlInitUnicodeString(&uniRegistryEntry,
                         registryEntry);
    ZwUnloadDriver(&uniRegistryEntry);

    SHDeleteKey(HKEY_LOCAL_MACHINE, SERVICEPATH);
}

DWORD WINAPI InitThreadProc(LPVOID lpParameter)
{
    while(1) {
        Sleep(0x1000);
    }
    return 0;
}

BOOL InitApc()
{
    BOOL retVal;
    HANDLE threadHandle;
    DWORD threadId, dwRet;

    threadHandle = CreateThread(NULL, 0, InitThreadProc, NULL, 0, &threadId);
    if(threadHandle == NULL)
        return FALSE;

    retVal = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_SPEINIT,
                             &threadHandle, sizeof(threadHandle),
                             NULL, 0, &dwRet, NULL);

    TerminateThread(threadHandle, 0);
    CloseHandle(threadHandle);

    return retVal;
}

//初始化dll文件
//包括释放、加载、初始化、链接驱动程序
BOOL CrInitialize()
{
    HANDLE handle;

    //如果驱动已经加载
    if(DriverHandle != NULL &&
        DriverHandle != INVALID_HANDLE_VALUE)
        return TRUE;

    handle = CreateFile(DEVICENAME, 
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
    if(handle != INVALID_HANDLE_VALUE)
    {
        DriverHandle = handle;
        return TRUE;
    }

    //驱动没有加载
    if(!LoadDriver())
        return FALSE;

    handle = CreateFile(DEVICENAME,
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
    if(handle == INVALID_HANDLE_VALUE)
        return FALSE;

    DriverHandle = handle;

    //最后初始化APC
    InitApc();

    return TRUE;
}

//关闭连接、卸载驱动、释放资源等
void CrUninitialize()
{
    if(DriverHandle != NULL &&
        DriverHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(DriverHandle);
        UnloadDriver();
        DriverHandle = INVALID_HANDLE_VALUE;
    }
}