#include <windows.h>
#include <stdio.h>
#include "..\CrArkKrnl\CrArkKrnl.h"

int main()
{
    PPROCESS_INFO_LIST infos;

    infos = CrGetProcInfos();
    if(infos == NULL)
        return 0;

    for(int i = 0; i < infos->cProcessInfos; ++i)
    {
        printf("pid:%lu\tname:%S\tpath:%S\n", infos->ProcInfo[i].dwPID,
                                              infos->ProcInfo[i].szProcName,
                                              infos->ProcInfo[i].szExePath);
    }

    return 0;
}