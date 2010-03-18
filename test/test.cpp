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
            processInfo = CrQueryProcessInfo(objIdTable->Entry[i].Object);
            if(processInfo)
            {
                printf("base: %d\thandle count: %d\n", processInfo->BasePriority, processInfo->HandleCount);
                delete processInfo;
            }
            objThreadTable = CrThreadEnum(objIdTable->Entry[i].Object);
            if(objThreadTable)
            {
                for(unsigned int j = 0; j < objThreadTable->Count; j++)
                {
                    printf("\tETHREAD: %8.8X\tTID:%lu\n", objThreadTable->Entry[j].Object, objThreadTable->Entry[j].UniqueId);
                }
                delete objThreadTable;
            }
        }
        delete objIdTable;
    }

    getchar();
    return 0;
}