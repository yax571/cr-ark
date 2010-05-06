#include "stdafx.h"
#include "Mem.h"
#include "MsgHook.h"
#include <vector>

typedef struct _MsgHookInfo {
    DWORD Handle;
    int idHook;
    PVOID Function;
    PCHAR ModuleName;
    PVOID EProcess;
    PVOID EThread;
}MsgHookInfo, *PMsgHookInfo;

typedef enum _HANDLE_TYPE
{
    TYPE_FREE = 0,
    TYPE_WINDOW = 1,
    TYPE_MENU = 2,
    TYPE_CURSOR = 3,
    TYPE_SETWINDOWPOS = 4,
    TYPE_HOOK = 5,
    TYPE_CLIPDATA = 6,
    TYPE_CALLPROC = 7,
    TYPE_ACCELTABLE = 8,
    TYPE_DDEACCESS = 9,
    TYPE_DDECONV = 10,
    TYPE_DDEXACT = 11,
    TYPE_MONITOR = 12,
    TYPE_KBDLAYOUT = 13,
    TYPE_KBDFILE = 14,
    TYPE_WINEVENTHOOK = 15,
    TYPE_TIMER = 16,
    TYPE_INPUTCONTEXT = 17,
    TYPE_CTYPES = 18,
    TYPE_GENERIC = 255
}HANDLE_TYPE;

typedef struct _HEAD
{
    HANDLE h;
    ULONG cLockObj;
}HEAD;

typedef struct _THROBJHEAD
{
    HEAD headinfo;
    PVOID pti; 
}THROBJHEAD;

typedef  struct _DESKHEAD
{
    PVOID rpdesk; 
    PBYTE pSelf ; 
}DESKHEAD;

typedef struct _THRDESKHEAD
{
    THROBJHEAD ThreadObjHead ;
    DESKHEAD DesktopHead ;
}THRDESKHEAD;

typedef  struct _HOOK
{
    THRDESKHEAD tshead ;
    struct _HOOK* phkNext ;
    int idHook;    
    ULONG offPfn;    
    UINT flags ;        
    DWORD  ihmod ;    
    PVOID ptiHooked;    
    PVOID rpdesk ;      
}HOOK,*PHOOK;

typedef struct _HANDLEENTRY{
    PVOID  phead;       
    ULONG  pOwner;      
    BYTE  bType;        
    BYTE  bFlags;       
    WORD  wUniq;       
}HANDLEENTRY,*PHANDLEENTRY;

typedef struct _SERVERINFO{   
    WORD wRIPFlags ;       
    WORD wSRVIFlags ;      
    WORD wRIPPID ;         
    WORD wRIPError ;       
    ULONG cHandleEntries;   
}SERVERINFO,*PSERVERINFO;

typedef struct _SHAREDINFO{
    PSERVERINFO psi;      
    PHANDLEENTRY aheList;          
    ULONG pDispInfo;      
    ULONG ulSharedDelta;  
}SHAREDINFO,*PSHAREDINFO; 

PMsgHookInfo WINAPI CrGetMsgHookInfo() {
    static PSHAREDINFO pSharedInfo = NULL;
    SERVERINFO serverInfo;
    HMODULE hUser32;
    PBYTE lpUserRegisterWowHandlers, currentPos;
    PHANDLEENTRY pHandleEntry = NULL;
    MsgHookInfo msgHookInfo;
    HOOK hook;
    std::vector<MsgHookInfo> msgHookInfoArray;
    DWORD i, dwRet;

    // 首先从user32.dll -> UserRegisterWowHandlers 中搜索
    // SharedInfo结构的指针
    if(pSharedInfo == NULL) {
        // 加载user32.dll
        hUser32 = GetModuleHandle(_T("user32.dll"));
        if(hUser32 == NULL)
            hUser32 = LoadLibrary(_T("user32.dll"));
        if(hUser32 == NULL)
            return NULL;

        // 获得UserRegisterWowHandlers的指针
        lpUserRegisterWowHandlers = (PBYTE)GetProcAddress(hUser32, "UserRegisterWowHandlers");
        if(lpUserRegisterWowHandlers == NULL)
            return NULL;

        // 搜索特征获得SharedInfo结构
        for (currentPos = lpUserRegisterWowHandlers; 
             currentPos <= lpUserRegisterWowHandlers +0x250; 
             ++currentPos )
        {
            if (0x40c7 == *(WORD*)currentPos && 
                0xb8 == *(BYTE*)(currentPos + 7))
            {
                pSharedInfo= (PSHAREDINFO)(*(DWORD*)(currentPos + 8));
                break;
            }
        }
    }

    // 获得SharedInfo->psi
    // 生成缓冲区
    memcpy(&serverInfo, pSharedInfo->psi, sizeof(serverInfo));
    pHandleEntry = (PHANDLEENTRY)(new BYTE[serverInfo.cHandleEntries * sizeof(HANDLEENTRY)]);
    if(pHandleEntry == NULL)
        return NULL;


    // 获取HandleEntry
    memcpy(pHandleEntry, pSharedInfo->aheList, serverInfo.cHandleEntries * sizeof(HANDLEENTRY));
    for(i = 0; i < serverInfo.cHandleEntries; ++i) {
        if(pHandleEntry[i].bType != TYPE_HOOK)
            continue;

        dwRet = ReadKernelMem(pHandleEntry[i].phead, &hook, sizeof(hook));
        if(dwRet != sizeof(hook)) 
            goto __ExitCrGetMsgHookInfo;

        msgHookInfo.idHook = hook.idHook;
        msgHookInfo.Function = (PVOID)hook.offPfn;
        msgHookInfo.Handle = (DWORD)hook.tshead.ThreadObjHead.headinfo.h;

    }

__ExitCrGetMsgHookInfo:
    if(pHandleEntry)
        delete[] pHandleEntry;
    return NULL;
}