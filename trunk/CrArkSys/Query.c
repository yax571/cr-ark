#include <ntifs.h>
#include "Undocument.h"
#include "Enviroment.h"
#include "ProcEnum.h"
#include "helper.h"
#include "Query.h"

//EPROCESS->Peb->Ldr->InLoadOrderModuleList.FLinkָ��
//_LDR_DATA_TABLE_ENTRY�ṹ  _LDR_DATA_TABLE_ENTRY->FullDllName����Exe�ļ�ȫ·��
BOOLEAN
QueryProcessFullPathByPeb(PProcessNameInfo ProcNameInfo,
                          PEPROCESS Process)
{
    BOOLEAN ret;
    BOOLEAN attached;
    KAPC_STATE oldApcState;
    PUCHAR magic;
    ANSI_STRING ansiString;
    PUNICODE_STRING unicodeString;
    NTSTATUS status;

    ret = FALSE;
    attached = FALSE;
    magic = (PUCHAR)Process;

    //������ǵ�ǰ����, �Ƚ���Ŀ����̵ĵ�ַ�ռ�
    if(Process != PsGetCurrentProcess())
    {
        KeStackAttachProcess(Process, &oldApcState);
        attached = TRUE;
    }

    //�û�̬�ռ䴦��
    try {
        //EPROCESS->Peb
        magic = *(PUCHAR*)(magic + EProcessPebOffset);
        
        //Peb->Ldr
        ProbeForRead(magic + 0x0c, 4, 1);
        magic = *(PUCHAR*)(magic + 0x0c);

        //Ldr->InLoadOrderModuleList.FLink
        ProbeForRead(magic + 0x0c, 4, 1);
        magic = *(PUCHAR*)(magic + 0x0c);

        //_LDR_DATA_TABLE_ENTRY.FullDllName
        ProbeForRead(magic + 0x24, sizeof(UNICODE_STRING), 1);
        magic = magic + 0x24;

        //����
        unicodeString = (PUNICODE_STRING)magic;
        RtlZeroMemory(ProcNameInfo->FullPath, FULL_PATH_LENGTH);
        ansiString.Buffer = ProcNameInfo->FullPath;
        ansiString.Length = 0;
        ansiString.MaximumLength = FULL_PATH_LENGTH;

        status = RtlUnicodeStringToAnsiString(&ansiString, 
                                              unicodeString, 
                                              FALSE);
        if(NT_SUCCESS(status))
            ret = TRUE;
    } except(EXCEPTION_EXECUTE_HANDLER) {
        ret = FALSE;
    }

    if(attached)
    {
        KeUnstackDetachProcess(&oldApcState);
    }

    return ret;
}


//EPROCESS->ImageFileName��������
//EPROCESS->SectionObject->Segment->SubSection->ControlArea->FilePointer��һ��FileObject
//������ļ��������ObQueryNameString���Ի��ȫ·��
//Ҳ���Լ�����IRP�·���ѯ
BOOLEAN
QueryProcessFullPathBySection(PProcessNameInfo ProcNameInfo,
                              PEPROCESS Process)
{
    PUCHAR magic;
    PFILE_OBJECT filePointer;
    POBJECT_NAME_INFORMATION objectNameInfo;
    UNICODE_STRING dosFullPath;
    NTSTATUS status;
    ULONG retLength;
    ANSI_STRING fullPath;

    magic = (PUCHAR)Process;

    //ȡ��FileObjectָ��
    //magic = EPROCESS.SectionObject
    if(MmIsAddressValid(magic + EProcessSectionObjectOffset) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic + EProcessSectionObjectOffset);

    //magic = SectionObject.Segment
    if(MmIsAddressValid(magic + SectionObjectSegmentOffset) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic + SectionObjectSegmentOffset);

    //magic = SegmentObject.SubSection
    if(MmIsAddressValid(magic + SegmentObjectSubSecOffset) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic + SegmentObjectSubSecOffset);

    //magic = SubSection.ControlArea
    if(MmIsAddressValid(magic) == FALSE)
        return FALSE;
    magic = *(PUCHAR*)(magic);

    //ControlArea.FilePointer
    if(MmIsAddressValid(magic + ControlAreaFilePointerOffset) == FALSE)
        return FALSE;
    filePointer = *(PFILE_OBJECT*)(magic + ControlAreaFilePointerOffset);

    //�������ü���, ��ɾ
    status = ObReferenceObjectByPointer(filePointer,
                                        0,
                                        *IoFileObjectType,
                                        KernelMode);
    if(!NT_SUCCESS(status))
        return FALSE;

    //���ڵõ���FileObjectָ��.....
    objectNameInfo = ExAllocatePoolWithTag(PagedPool,
                                           1024,
                                           'obni');
    if(objectNameInfo == NULL)
        return FALSE;
    status = ObQueryNameString(filePointer,
                               objectNameInfo,
                               1024,
                               &retLength);
    if(!NT_SUCCESS(status) || retLength > 1024)
    {
        ExFreePool(objectNameInfo);
        ObDereferenceObject(filePointer);
        return FALSE;
    }

    fullPath.Buffer = ProcNameInfo->FullPath;
    fullPath.Length = 0;
    fullPath.MaximumLength = FULL_PATH_LENGTH;
    RtlZeroMemory(ProcNameInfo->FullPath, FULL_PATH_LENGTH);

    //unicodeת��Ϊansi
    if(ConvertDevicePathToDosPath(&objectNameInfo->Name,
                                  TRUE,
                                  PagedPool,
                                  &dosFullPath))
    {
        status = RtlUnicodeStringToAnsiString(&fullPath,
                                              &dosFullPath,
                                              FALSE);
        ExFreePool(dosFullPath.Buffer);
    }
    else
    {
        status = RtlUnicodeStringToAnsiString(&fullPath,
                                              &objectNameInfo->Name,
                                              FALSE);
    }
    if(!NT_SUCCESS(status))
    {
        ExFreePool(objectNameInfo);
        ObDereferenceObject(filePointer);
        return FALSE;
    }

    ExFreePool(objectNameInfo);
    ObDereferenceObject(filePointer);
    return TRUE;
}


//����EProcess�ṹ��þ������ƺ�·��
//ʧ�ܷ���NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process)
{
    PProcessNameInfo procNameInfo;

    if(!IsProcessObject(Process))
    {
        KdPrint(("QueryProcessName: Param Not A EPROCESS\n"));
        return NULL;
    }

    procNameInfo = ExAllocatePoolWithTag(PagedPool,
                                         sizeof(ProcessNameInfo),
                                         'pnin');
    if(procNameInfo == NULL)
    {
        KdPrint(("allocate ProcessNameInfo fail.\n"));
        return NULL;
    }

    //ϵͳ���̺������˳��Ľ������⴦��
    if(Process == SystemProcess)
    {
        RtlCopyMemory(procNameInfo->ImageName, "[System]", 8);
        procNameInfo->ImageName[8] = 0;
        RtlCopyMemory(procNameInfo->FullPath, "NT System Kernel", 16);
        procNameInfo->FullPath[16] = 0;

        return procNameInfo;
    }
    else if(IsProcessDeleting(Process))
    {
        RtlCopyMemory(procNameInfo->ImageName, "Deleting", 8);
        procNameInfo->ImageName[8] = 0;
        RtlCopyMemory(procNameInfo->FullPath, "Deleting", 8);
        procNameInfo->FullPath[8] = 0;

        return procNameInfo;
    }

    //ERPCESS.ImageFileName��þ�������
    if(MmIsAddressValid((PUCHAR)Process + EProcessImageFileNameOffset) == FALSE)
    {
        ExFreePool(procNameInfo);
        return NULL;
    }
    RtlCopyMemory(procNameInfo->ImageName, 
                  (PUCHAR)Process + EProcessImageFileNameOffset,
                  16);
    procNameInfo->ImageName[16] = 0;

    //����ļ�ȫ·��
    if(QueryProcessFullPathBySection(procNameInfo, Process))
        return procNameInfo;

    if(QueryProcessFullPathByPeb(procNameInfo, Process))
        return procNameInfo;

    ExFreePool(procNameInfo);
    return NULL;
}