#include "Mem.h"
#include "MsgHook.h"

typedef struct _MsgHookInfo {
    DWORD Handle;
    BYTE Type;
    PVOID Function;
    PCHAR ModuleName;
    PVOID EProcess;
    PVOID EThread;
}MsgHookInfo, *PMsgHookInfo;

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
        lpUserRegisterWowHandlers = GetProcAddress(hUser32, "UserRegisterWowHandlers");
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

    memcpy(&serverInfo, pSharedInfo->psi, sizeof(serverInfo));

    return NULL;
}