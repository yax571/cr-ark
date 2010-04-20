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

    //����ں˻���ַ
    KernelImgBase = sysMod->Module[0].Base;

    //��ϳ��ں��ļ�·��
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

    //���ں˸�������ӳ�䵽������
    if(KernelHandle == NULL && LoadKernelImg() == FALSE) {
       return NULL;
    }

    //����ļ�ͷ
    dosHeader = (PIMAGE_DOS_HEADER)KernelHandle;
    ntHeader = (PIMAGE_NT_HEADERS)(dosHeader->e_lfanew + (DWORD)KernelHandle);

    //���ں���ssdt��λ��
    offset = (DWORD)GetProcAddress(KernelHandle, "KeServiceDescriptorTable");
    if(offset == NULL)
        return NULL;
    offset -= (DWORD)KernelHandle;
    ssdtAddress = offset + (DWORD)KernelImgBase;

    //���ں���KiServiceTable��ַ
    dwRet = ReadKernelMem((PVOID)ssdtAddress, &remoteSSDT, sizeof(remoteSSDT));
    if(dwRet != sizeof(remoteSSDT))
        return NULL;

    //���㱾�ص�KiServiceTable��ַ
    localKiServiceTable = (DWORD)remoteSSDT.ServiceRoutineTable - (DWORD)KernelImgBase + (DWORD)KernelHandle;

    serviceTableInfo = new ServiceTableInfo;
    if(serviceTableInfo == NULL)
        return NULL;

    //��ȡԭʼKiServiceTable�͵�ǰ��KiServiceTable
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

