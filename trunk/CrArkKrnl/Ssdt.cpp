#include "stdafx.h"
#include "CrArkKrnl.h"
#include "Mem.h"
#include "ntapi.h"
#include "Ssdt.h"

typedef struct _ServiceDescriptorTable {
    PVOID* ServiceRoutineTable;
    DWORD  UnUsed;
    DWORD  Count;
    DWORD* ServiceParamTable;
}ServiceDescriptorTable, *PServiceDescriptorTable;

HMODULE KernelHandle = NULL;
char KernelImgName[256];
char Win32kImgName[256];
PVOID KernelImgBase;
PVOID Win32kImgBase;

BOOL LoadKernelImg() {
    PVOID buffer;
    NTSTATUS status;
    DWORD dwLength = 0;
    BOOL ret = FALSE;
    PSYSTEM_MODULE_INFORMATION sysMod;
    char *start;

    if(KernelHandle != NULL)
        return TRUE;

    ZwQuerySystemInformation(SystemModuleInformation,
                             NULL, 0, &dwLength);
    if(dwLength == 0) 
        goto _LoadKernelImgExit;
    
    buffer = new char[dwLength];
    if(buffer == NULL) 
        goto _LoadKernelImgExit;

    status = ZwQuerySystemInformation(SystemModuleInformation,
                                      buffer, dwLength, &dwLength);
    if(!NT_SUCCESS(status)) 
        goto _LoadKernelImgExit;

    sysMod = (PSYSTEM_MODULE_INFORMATION)buffer;

    //获得内核基地址
    KernelImgBase = sysMod->Module[0].Base;

    //组合出内核文件路径
    dwLength = GetSystemDirectoryA(KernelImgName, sizeof(KernelImgName));
    if(KernelImgName[dwLength - 1] != '\\') {
        KernelImgName[dwLength++] = '\\';
        KernelImgName[dwLength] = 0;
    }
    start = sysMod->Module[0].ImageName + sysMod->Module[0].PathLength;
    if(*start == '\\') start++;
    while(*start) {
        KernelImgName[dwLength++] = *start;
        start++;
    }
    KernelImgName[dwLength] = 0;

    KernelHandle = LoadLibraryA(KernelImgName);
    if(KernelHandle == NULL)
        goto _LoadKernelImgExit;

    ret = TRUE;
_LoadKernelImgExit:
    if(buffer)
        delete[] buffer;

    return ret;
}

PServiceTableInfo WINAPI CrGetSSDTInfo() {
    DWORD offset, ssdtAddress;
    ServiceDescriptorTable remoteSSDT;
    DWORD dwRet, i;
    DWORD localKiServiceTable;
    PServiceTableInfo serviceTableInfo;
    PIMAGE_DOS_HEADER dosHeader;
    PIMAGE_NT_HEADERS ntHeader;

    //若内核副本不能映射到本进程
    if(KernelHandle == NULL && LoadKernelImg() == FALSE) {
       return NULL;
    }

    //获得文件头
    dosHeader = (PIMAGE_DOS_HEADER)KernelHandle;
    ntHeader = (PIMAGE_NT_HEADERS)(dosHeader->e_lfanew + (DWORD)KernelHandle);

    //求内核中ssdt的位置
    offset = (DWORD)GetProcAddress(KernelHandle, "KeServiceDescriptorTable");
    if(offset == NULL)
        return NULL;
    offset -= (DWORD)KernelHandle;
    ssdtAddress = offset + (DWORD)KernelImgBase;

    //读内核中KiServiceTable地址
    dwRet = ReadKernelMem((PVOID)ssdtAddress, &remoteSSDT, sizeof(remoteSSDT));
    if(dwRet != sizeof(remoteSSDT))
        return NULL;

    //计算本地的KiServiceTable地址
    localKiServiceTable = (DWORD)remoteSSDT.ServiceRoutineTable - (DWORD)KernelImgBase + (DWORD)KernelHandle;

    serviceTableInfo = new ServiceTableInfo;
    if(serviceTableInfo == NULL)
        return NULL;

    //读取原始KiServiceTable和当前的KiServiceTable
    serviceTableInfo->Count = remoteSSDT.Count;
    memcpy(&serviceTableInfo->OriginAddress, (PVOID)localKiServiceTable, serviceTableInfo->Count * sizeof(PVOID));
    for(i = 0; i < serviceTableInfo->Count; i++) {
        serviceTableInfo->OriginAddress[i] = 
            (PVOID)((DWORD)serviceTableInfo->OriginAddress[i] - ntHeader->OptionalHeader.ImageBase + (DWORD)KernelImgBase);
    }
    dwRet = ReadKernelMem(remoteSSDT.ServiceRoutineTable, &serviceTableInfo->CurrentAddress, serviceTableInfo->Count * sizeof(PVOID));
    if(dwRet != serviceTableInfo->Count * sizeof(PVOID)) {
        delete serviceTableInfo;
        return NULL;
    }

    return serviceTableInfo;
}

