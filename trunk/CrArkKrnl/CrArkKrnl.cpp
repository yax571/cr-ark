// CrArkKrnl.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "CrArkKrnl.h"
#include <TlHelp32.h>
#include <Psapi.h>

BOOL GetExePathFromPid(DWORD dwPid, WCHAR* szFilePath, DWORD nSize)
{
    HANDLE hProc;
    DWORD dwRet;
    BOOL ret;

    hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                        FALSE,
                        dwPid);
    if(hProc == NULL)
        return FALSE;

    //exe�ļ�·��
    dwRet = GetModuleFileNameExW(hProc,
                                 NULL,
                                 szFilePath,
                                 nSize);
    CloseHandle(hProc);

    return (dwRet > 0);
}

CRARKKRNL_API PPROCESS_INFO_LIST CrGetProcInfos()
{
    HANDLE hSnap;
    DWORD dwProcCount;
    PROCESSENTRY32W pe32;
    PPROCESS_INFO_LIST procInfoList = NULL;
    BOOL ret = FALSE;
    BOOL bGetPath;
    HANDLE hProc;

    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,
                                     0);
    __try{
        if(hSnap == INVALID_HANDLE_VALUE)
            __leave;

        //��һ�α�������, ��ý�������
        dwProcCount = 0;

        memset(&pe32, 0, sizeof(pe32));
        pe32.dwSize = sizeof(pe32);

        ret = Process32FirstW(hSnap,
                             &pe32);
        while(ret)
        {
            ++dwProcCount;
            ret = Process32NextW(hSnap,
                                &pe32);
        }

        //����ռ�
        procInfoList = (PPROCESS_INFO_LIST)new BYTE[sizeof(PROCESS_INFO_LIST) + 
                                                    sizeof(PROCESS_INFO) * (dwProcCount - 1)];
        if(!procInfoList)
        {
            ret = FALSE;
            __leave;
        }
        procInfoList->cProcessInfos = dwProcCount;

        //�رվ��
        CloseHandle(hSnap);

        //�ڶ���ȡ��, ��ý�����Ϣ
        hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,
                                         0);
        if(hSnap == INVALID_HANDLE_VALUE)
        {
            ret = FALSE;
            __leave;
        }

        memset(&pe32, 0, sizeof(pe32));
        pe32.dwSize = sizeof(pe32);
        
        ret = Process32FirstW(hSnap,
                             &pe32);
        for(DWORD i = 0; (i < dwProcCount) && (ret); ++i)
        {
            //����ID
            procInfoList->ProcInfo[i].dwPID = pe32.th32ProcessID;

            //������
            wcscpy(procInfoList->ProcInfo[i].szProcName,
                   pe32.szExeFile);
            
            //��ȡ����·��
            bGetPath = GetExePathFromPid(pe32.th32ProcessID,
                                         procInfoList->ProcInfo[i].szExePath,
                                         512);
            if(!bGetPath)
            {
                wcscpy(procInfoList->ProcInfo[i].szExePath,
                       L"���ܻ����ϸ·��");
            }

            ret = Process32NextW(hSnap,
                                &pe32);
        }

        ret = TRUE;
    }
    __finally{
        if(hSnap != INVALID_HANDLE_VALUE)
            CloseHandle(hSnap);

        if(ret == FALSE &&
           procInfoList != NULL)
        {
           delete[] procInfoList;
           procInfoList = NULL;
        }
    }

    return procInfoList;
}