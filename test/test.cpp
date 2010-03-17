#include <windows.h>
#include <stdio.h>
#include "..\CrArkKrnl\CrArkKrnl.h"

int main()
{
    if(CrInitialize())
        printf("initialize ok..\n");
    else
        printf("fail.\n");

    getchar();
    return 0;
}