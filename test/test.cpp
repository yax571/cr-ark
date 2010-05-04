#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "..\CrArkKrnl\CrArkKrnl.h"

int main() {
    BOOL bRet;
    PServiceTableInfo tableInfo;
    DWORD i;

    bRet = CrInitialize();
    if(bRet == FALSE) {
        printf("initialize fail.\n");
        return 0;
    }

    tableInfo = CrGetSSDTInfo();
    if(tableInfo == NULL) {
        printf("CrGetSSDTInfo fail.\n");
        CrUninitialize();
        return 0;
    }

    printf("no\tkernel\torigin\n");
    for(i = 0; i < tableInfo->Count; ++i) {
        printf("%d\t%8.8X\t%8.8X\n", i, tableInfo->CurrentAddress[i], tableInfo->OriginAddress[i]);
    }
    CrFreeMem(tableInfo);
}