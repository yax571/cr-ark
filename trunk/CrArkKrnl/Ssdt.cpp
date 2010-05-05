#include "stdafx.h"
#include "CrArkKrnl.h"
#include "Mem.h"
#include "ntapi.h"
#include "Ssdt.h"
#include "stdio.h"

typedef struct _ServiceDescriptorTable {
    PVOID* ServiceRoutineTable;
    DWORD  UnUsed;
    DWORD  Count;
    DWORD* ServiceParamTable;
}ServiceDescriptorTable, *PServiceDescriptorTable;

BOOL SetFunctionModuleName(PServiceTableInfo serviceTableInfo);
BOOL GetFunctionName(PServiceTableInfo serviceTableInfo);

HMODULE KernelHandle = NULL;
char KernelImgName[256];
char Win32kImgName[256];
PVOID KernelImgBase;
PVOID Win32kImgBase;
PVOID RemoteKiServiceTable = NULL;
PVOID RemoteParamTable = NULL;
PSYSTEM_MODULE_INFORMATION SystemModuleInfo;

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
    RemoteKiServiceTable = remoteSSDT.ServiceRoutineTable;

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

    //��ԭʼparam table
    dwRet = ReadKernelMem(remoteSSDT.ServiceParamTable, &serviceTableInfo->ParamTable, serviceTableInfo->Count * sizeof(DWORD));
    if(dwRet != serviceTableInfo->Count * sizeof(DWORD)) {
        delete serviceTableInfo;
        return NULL;
    }

    SetFunctionModuleName(serviceTableInfo);
    GetFunctionName(serviceTableInfo);

    return serviceTableInfo;
}

BOOL WINAPI CrFixSsdt(DWORD ServiceNumber, LPVOID Address) {
    DWORD dwRet;
    if(RemoteKiServiceTable == NULL)
        return FALSE;

    dwRet = WriteKernelMem(&Address, 
                           (PUCHAR)RemoteKiServiceTable + 4 * ServiceNumber, 4);
    if(dwRet != 4)
        return FALSE;

    return TRUE;
}

//��ȡ��ǰ������Ӧ��ģ��
BOOL SetFunctionModuleName(PServiceTableInfo serviceTableInfo) {
    NTSTATUS status;
    DWORD dwLength = 0, dwLength2, i, j, address, index;


    memset(serviceTableInfo->CurrentModuleName, 0, 
           sizeof(serviceTableInfo->CurrentModuleName));

    ZwQuerySystemInformation(SystemModuleInformation, NULL, 0, &dwLength);
    if(dwLength == 0)
        return FALSE;

    if(SystemModuleInfo != NULL)
        delete[] SystemModuleInfo;

    SystemModuleInfo = (PSYSTEM_MODULE_INFORMATION)(new BYTE[dwLength]);
    status = ZwQuerySystemInformation(SystemModuleInformation,
                                      SystemModuleInfo,
                                      dwLength,
                                      &dwLength2);
    if(!NT_SUCCESS(status)) {
        delete[] SystemModuleInfo;
        SystemModuleInfo = NULL;
        return FALSE;
    }

    for(i = 0; i < serviceTableInfo->Count; ++i) {
        address = (DWORD)serviceTableInfo->CurrentAddress[i];
        index = -1;
        for(j = 0; j < SystemModuleInfo->Count; ++j) {
            if(address >= (DWORD)SystemModuleInfo->Module[j].Base &&
               address <= (DWORD)SystemModuleInfo->Module[j].Base + 
                           SystemModuleInfo->Module[j].Size) {
               index = j;
               break;
            }
        }

        if(index == -1)
            continue;

        serviceTableInfo->CurrentModuleName[i] = SystemModuleInfo->Module[index].ImageName;
    }

    return TRUE;
}


DWORD GetServiceNumberFromFunction(PVOID Function) {
    DWORD dwRet = -1, magic;
    PUCHAR lpFunction = (PUCHAR)Function;

    if(lpFunction[0] != 0xB8) 
        return dwRet;

    lpFunction += 5;
    if(lpFunction[0] != 0xBA)
        return dwRet;

    magic = *(DWORD*)(lpFunction + 1);
    if(magic <= 0x70000000)
        return dwRet;

    lpFunction += 5;
    magic = *(WORD*)lpFunction;
    if(magic != 0x12FF)
        return dwRet;

    lpFunction += 2;
    if(lpFunction[0] != 0xC2)
        return dwRet;

    lpFunction = (PUCHAR)Function;
    dwRet = *(DWORD*)(lpFunction + 1);

    return dwRet;
}

//��ȡ����Ŷ�Ӧ�ĺ�����
BOOL GetFunctionName(PServiceTableInfo serviceTableInfo) {
    PVOID ntdllBase, lpFunction;
    DWORD imageBase, numberOfFunction, numberOfName, i, index, serviceNo;
    PDWORD addressOfFunctionRva, addressOfNamesRva;
    PWORD nameOrdinals;
    PCHAR functionName;
    PIMAGE_DOS_HEADER dosHeader;
    PIMAGE_NT_HEADERS peHeaders;
    PIMAGE_DATA_DIRECTORY exportDirectoryEntry;
    PIMAGE_EXPORT_DIRECTORY exportDirectory;

    memset(serviceTableInfo->FunctionName, 0, 
        sizeof(serviceTableInfo->FunctionName));

    ntdllBase = GetModuleHandle(_T("ntdll.dll"));
    dosHeader = (PIMAGE_DOS_HEADER)ntdllBase;

    if(ntdllBase == NULL)
        return FALSE;

    //��֤�Ƿ���PE�ļ�
    peHeaders = (PIMAGE_NT_HEADERS)((DWORD)ntdllBase + dosHeader->e_lfanew);
    if(peHeaders->Signature != 'EP')
        return FALSE;

    //��õ�����
    imageBase = peHeaders->OptionalHeader.ImageBase;
    exportDirectoryEntry = &
        peHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    exportDirectory = (PIMAGE_EXPORT_DIRECTORY)
        (imageBase + exportDirectoryEntry->VirtualAddress);

    //��õ��������Ϣ
    numberOfFunction = exportDirectory->NumberOfFunctions;
    numberOfName = exportDirectory->NumberOfNames;
    nameOrdinals = (PWORD)(imageBase + exportDirectory->AddressOfNameOrdinals);
    addressOfFunctionRva = (PDWORD)
        (imageBase + exportDirectory->AddressOfFunctions);
    addressOfNamesRva = (PDWORD)
        (imageBase + exportDirectory->AddressOfNames);

    //�����������ú�����
    for(i = 0; i < numberOfName; ++i) {
        index = nameOrdinals[i];
        if(index >= numberOfFunction)
            continue;
    
        functionName = (PCHAR)(addressOfNamesRva[i] + imageBase);
        lpFunction = (PVOID)(addressOfFunctionRva[index] + imageBase);

        //�жϺ����ķ����
        serviceNo = GetServiceNumberFromFunction(lpFunction);
        if(serviceNo == -1)
            continue;

        serviceTableInfo->FunctionName[serviceNo] = functionName;
    }

    return TRUE;
}