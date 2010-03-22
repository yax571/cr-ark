#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "..\CrArkKrnl\CrArkKrnl.h"

char Cmd[128];
PObjectIdTable ProcessList;

void UpdateProcessList() 
{
    if(ProcessList)
        delete ProcessList;

    ProcessList = CrProcessEnum(TRUE);
}

void ListProcess() {
    PProcessNameInfo processNameInfo;
    int i;

    UpdateProcessList();

    if(ProcessList) {
        for(i = 0; i < ProcessList->Count; i++) {
            printf("EPROCESS: %8.8X  PID: %lu ", ProcessList->Entry[i].Object, ProcessList->Entry[i].UniqueId);
            processNameInfo = CrQueryProcessName(ProcessList->Entry[i].Object);
            if(processNameInfo) {
                printf("ImageName: %s\n\tFullPath: %s\n", processNameInfo->ImageName, processNameInfo->FullPath);
                delete processNameInfo;
            }
        }
    }
}

void ListProcessInfo() 
{
    int pid, i, j;
    PProcessInfo processInfo;
    PObjectIdTable threadTable;

    UpdateProcessList();

    printf("pid: ");
    scanf("%d", &pid);
    for(i = 0; i < ProcessList->Count; i++) {
        if(ProcessList->Entry[i].UniqueId == pid) {
            processInfo = CrQueryProcessInfo(ProcessList->Entry[i].Object);
            if(!processInfo)
                break;
            printf("Pid: %lu ParentPid: %lu HandleCount:%lu\n", processInfo->UniqueProcessId,
                               processInfo->InheritedFromUniqueProcessId,
                               processInfo->HandleCount);
            delete processInfo;

            threadTable = CrThreadEnum(ProcessList->Entry[i].Object);
            if(!threadTable)
                break;
            for(j = 0; j < threadTable->Count; j++) {
                printf("\tETHREAD: %8.8X\tTID: %lu\n", threadTable->Entry[j].Object, threadTable->Entry[j].UniqueId);
            }
            delete threadTable;
            break;
        }
    }
}

void TerminateThread()
{
    int pid, tid, force;
    PObjectIdTable threadTable;

    UpdateProcessList();

    printf("PID: ");
    scanf("%d", &pid);
    printf("TID: ");
    scanf("%d", &tid);
    printf("Force End: ");
    scanf("%d", &force);

    for(int i = 0; i < ProcessList->Count; i++) {
        if(ProcessList->Entry[i].UniqueId == pid) {
            threadTable = CrThreadEnum(ProcessList->Entry[i].Object);
            if(threadTable)
            {
                for(int j = 0; j < threadTable->Count; j++) {
                    if(threadTable->Entry[j].UniqueId == tid) {
                        CrTerminateThread(threadTable->Entry[j].Object, 0, force);
                        break;
                    }
                }
                delete threadTable;
            }
            break;
        }
    }
}

void TerminateProcess()
{
    int pid, i;
    printf("pid: ");
    scanf("%d", &pid);
    
    UpdateProcessList();
    for(i = 0; i < ProcessList->Count; i++) {
        if(ProcessList->Entry[i].UniqueId == pid)
        {
            CrTerminateProcess(ProcessList->Entry[i].Object, 0, TRUE);
            break;
        }
    }
}

int main()
{
    if(!CrInitialize())
    {
        fprintf(stderr, "can not initialize driver.\n");
        return 0;
    }

    ProcessList = NULL;

    while(1) {
        printf("cmd> ");
        scanf("%s", Cmd);
        if(stricmp(Cmd, "Exit") == 0) {
            break;
        }
        else if(stricmp(Cmd, "ListProcess") == 0) {
            ListProcess();
        }
        else if(stricmp(Cmd, "ListProcessInfo") == 0) {
            ListProcessInfo();
        }
        else if(stricmp(Cmd, "TerminateThread") == 0) {
            TerminateThread();
        }
        else if(stricmp(Cmd, "TerminateProcess") == 0) {
            TerminateProcess();
        }
    }

    CrUninitialize();
    return 0;
}