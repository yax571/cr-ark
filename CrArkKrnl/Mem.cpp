#include "stdafx.h"
#include <WinIoCtl.h>
#include "CrArkKrnl.h"
#include "..\CrArkSys\CtrlCode.h"

DWORD WINAPI ReadKernelMem(PVOID BaseAddress, PVOID DestAddress, DWORD Length)
{
    DWORD dwRet = 0;
    BOOL bRet;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_READMEM,
                           &BaseAddress, sizeof(BaseAddress),
                           DestAddress, Length,
                           &dwRet, NULL);

    return dwRet;
}

DWORD WINAPI WriteKernelMem(PVOID SourceAddress, PVOID DestAddress, DWORD Length)
{
    DWORD input[3];
    BOOL bRet;
    DWORD dwRet = 0;

    input[0] = (DWORD)SourceAddress;
    input[1] = (DWORD)DestAddress;
    input[2] = Length;

    bRet = DeviceIoControl(DriverHandle, IOCTL_CRARKSYS_WRITEMEM,
                           input, sizeof(input),
                           NULL, 0, &dwRet, NULL);

    return dwRet;
}