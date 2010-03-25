#include <ntifs.h>
#include "Protect.h"
#include "ade32.h"
#include "helper.h"
#include "HashTable.h"

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

    HashTableInitialize(&ProtectObject);
    KeInitializeSpinLock(&ProtectObjectLock);
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