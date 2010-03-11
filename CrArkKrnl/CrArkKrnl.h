// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CRARKKRNL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CRARKKRNL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CRARKKRNL_EXPORTS
#define CRARKKRNL_API __declspec(dllexport)
#else
#define CRARKKRNL_API __declspec(dllimport)
#endif

typedef struct _PROCESS_INFO{
    WCHAR szProcName[32];
    DWORD dwPID;
    WCHAR szExePath[512];
}PROCESS_INFO, *PPROCESS_INFO;

typedef struct _PROCESS_INFO_LIST{
    DWORD cProcessInfos;
    PROCESS_INFO ProcInfo[1];
}PROCESS_INFO_LIST, *PPROCESS_INFO_LIST;

CRARKKRNL_API PPROCESS_INFO_LIST CrGetProcInfos();