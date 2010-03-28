#include <ntifs.h>
#include <string.h>
#include "Protect.h"
#include "ade32.h"
#include "helper.h"
#include "HashTable.h"
#include "ProcEnum.h"
#include "Query.h"

PEPROCESS CsrssProcess;
UCHAR ObReferenceObjectByHandleJmpBack[20];
HashTable ProtectObject;
KSPIN_LOCK ProtectObjectLock;
BOOLEAN ProtectInit;

NTSTATUS 
FakeObReferenceObjectByHandle(
                          IN HANDLE  Handle,
                          IN ACCESS_MASK  DesiredAccess,
                          IN POBJECT_TYPE  ObjectType  OPTIONAL,
                          IN KPROCESSOR_MODE  AccessMode,
                          OUT PVOID  *Object,
                          OUT POBJECT_HANDLE_INFORMATION  HandleInformation  OPTIONAL
                          )
{
    NTSTATUS status;
    KIRQL oldIrql;

    __asm {
        push HandleInformation;
        push Object;
        movzx eax, AccessMode;
        push eax;
        push ObjectType;
        push DesiredAccess;
        push Handle;
        lea eax, ObReferenceObjectByHandleJmpBack;
        call eax;
        mov status, eax;
    }

    if(!NT_SUCCESS(status))
        return status;

    if(!IsProcessObject(*Object) &&
        !IsThreadObject(*Object))
        return status;

    if(IsProcessObject(*Object)) {
        KeAcquireSpinLock(&ProtectObjectLock, &oldIrql);
        if(PsGetCurrentProcess() == *Object ||
           PsGetCurrentProcess() == CsrssProcess ||
           !HashTableCheck(&ProtectObject, (ULONG)*Object))
        {
           KeReleaseSpinLock(&ProtectObjectLock, oldIrql);
           return status;
        }

        KeReleaseSpinLock(&ProtectObjectLock, oldIrql);
        ObDereferenceObject(*Object);
        *Object = NULL;
        return STATUS_ACCESS_DENIED;
    }
    else if(IsThreadObject(*Object)) {
        KeAcquireSpinLock(&ProtectObjectLock, &oldIrql);
        if(PsGetCurrentProcess() == IoThreadToProcess(*Object) ||
           PsGetCurrentProcess() == CsrssProcess ||
           !HashTableCheck(&ProtectObject, (ULONG)*Object))
        {
           KeReleaseSpinLock(&ProtectObjectLock, oldIrql);
           return status;
        }

        KeReleaseSpinLock(&ProtectObjectLock, oldIrql);
        ObDereferenceObject(*Object);
        *Object = NULL;
        return STATUS_ACCESS_DENIED;
    }

    return status;
}



BOOLEAN
ProtectInitialize()
{
    BOOLEAN bRet;
    PObjectIdTable processTable;
    PProcessNameInfo nameInfo;
    ANSI_STRING ansiImageName;
    ANSI_STRING ansiFullPath;
    ANSI_STRING ansiCsrssPath;
    ANSI_STRING ansiCsrss;
    ULONG i;

    KdPrint(("enter ProtectInitialize\n"));
    HashTableInitialize(&ProtectObject);
    KeInitializeSpinLock(&ProtectObjectLock);

    //查找csrss.exe进程的EPROCESS
    processTable = ProcessEnum(FALSE);
    if(processTable == NULL) {
        return FALSE;
    }

    RtlInitAnsiString(&ansiCsrss, "CSRSS.EXE");
    RtlInitAnsiString(&ansiCsrssPath, "SYSTEM32\\CSRSS.EXE");
    bRet = FALSE;
    for(i = 0; i < processTable->Count; i++) {
        nameInfo = QueryProcessName((PEPROCESS)processTable->Entry[i].Object);
        if(nameInfo) {
            RtlInitAnsiString(&ansiImageName, nameInfo->ImageName);
            RtlInitAnsiString(&ansiFullPath, nameInfo->FullPath);
            if(RtlCompareString(&ansiImageName, &ansiCsrss, TRUE) == 0 &&
               ansiFullPath.Length > ansiCsrssPath.Length)
            {
                ansiFullPath.Buffer += ansiFullPath.Length - ansiCsrssPath.Length;
                ansiFullPath.Length = ansiFullPath.MaximumLength = ansiCsrssPath.Length;
                if(RtlCompareString(&ansiFullPath, &ansiCsrssPath, TRUE) == 0)
                {
                    CsrssProcess = (PEPROCESS)processTable->Entry[i].Object;
                    bRet = TRUE;
                }
            }
            ExFreePool(nameInfo);
        }
        if(bRet == TRUE)
            break;
    }
    ExFreePool(processTable);
    if(bRet == FALSE)
        return bRet;

    //挂钩函数
    bRet = HookFunction(ObReferenceObjectByHandle, 
                        FakeObReferenceObjectByHandle, 
                        (PUCHAR)ObReferenceObjectByHandleJmpBack);
    if(bRet)
        ProtectInit = TRUE;
    else
        ProtectInit = FALSE;

    return bRet;
}

VOID
ProtectCleanup()
{
    if(!ProtectInit)
        return;

    UnhookFunction(ObReferenceObjectByHandle,
                   ObReferenceObjectByHandleJmpBack);
    HashTableInitialize(&ProtectObject);
    ProtectInit = FALSE;
}

VOID
ProtectAddObject(IN PVOID Object,
                IN BOOLEAN Remove)
{
    KIRQL oldIrql;

    KeAcquireSpinLock(&ProtectObjectLock, &oldIrql);
    if(Remove) {
        HashTableDelete(&ProtectObject, (ULONG)Object);
    }
    else {
        HashTableMark(&ProtectObject, (ULONG)Object);
    }
    KeReleaseSpinLock(&ProtectObjectLock, oldIrql);
}