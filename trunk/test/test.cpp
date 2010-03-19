#include <windows.h>
#include <stdio.h>
#include "..\CrArkKrnl\CrArkKrnl.h"

int main()
{
    if(CrInitialize())
        printf("initialize ok..\n");
    else
    {
        printf("fail.\n");
        return 0;
    }

    PObjectIdTable objIdTable;
    PObjectIdTable objThreadTable;
    PProcessNameInfo processNameInfo;
    PProcessInfo processInfo;
    PThreadInfo threadInfo;
    PProcessModuleList modList;
    PModuleInfo modInfo;

    objIdTable = CrProcessEnum(TRUE);
    if(objIdTable)
    {
        for(unsigned int i = 0; i < objIdTable->Count; i++)
        {
            printf("EPROCESS: %8.8X\tPID:%lu\n", objIdTable->Entry[i].Object, objIdTable->Entry[i].UniqueId);
            processNameInfo = CrQueryProcessName(objIdTable->Entry[i].Object);
            if(processNameInfo)
            {
                printf("\tName: %s\tFullPath: %s\n", processNameInfo->ImageName, processNameInfo->FullPath);
                delete processNameInfo;
            }
            modList = CrQueryProcessModuleList(objIdTable->Entry[i].Object);
            if(modList)
            {
                for(int j = 0; j < modList->Count; j++)
                {
                    modInfo = CrQueryModuleInfo(modList->Process, modList->LdrDataTable[j]);
                    if(modInfo)
                    {
                        printf("\t Module: Base Address: %8.8X, EntryPoint: %8.8X, SizeOfImage: %lu, FullPath: %s\n", 
                            modInfo->BaseAddress, modInfo->EntryPoint, modInfo->SizeOfImage, modInfo->FullPath);
                        delete modInfo;
                    }
                }
                delete modList;
            }
        }
        delete objIdTable;
    }

    getchar();
    return 0;
}