#include <ntddk.h>
#include "ProcEnum.h"
#include "HashTable.h"
#include "Undocument.h"

typedef struct _EnumContext{
    BOOLEAN showDeleting;
    PHashTable hashT;
    PObjectIdTable objIdT;
}EnumContext, *PEnumContext;

BOOLEAN 
IsProcessDeleting(PVOID EProcess)
{
    ULONG signalState;

    if(MmIsAddressValid((PUCHAR)EProcess + 0x04) == FALSE)
        return TRUE;

    signalState = *(PULONG)((PUCHAR)EProcess + 0x04);
    return signalState;
}

//kd> dt _OBJECT_HEADER
// nt!_OBJECT_HEADER
// +0x000 PointerCount     : Int4B
// +0x004 HandleCount      : Int4B
// +0x004 NextToFree       : Ptr32 Void
// +0x008 Type             : Ptr32 _OBJECT_TYPE
// +0x00c NameInfoOffset   : UChar
// +0x00d HandleInfoOffset : UChar
// +0x00e QuotaInfoOffset  : UChar
// +0x00f Flags            : UChar
// +0x010 ObjectCreateInfo : Ptr32 _OBJECT_CREATE_INFORMATION
// +0x010 QuotaBlockCharged : Ptr32 Void
// +0x014 SecurityDescriptor : Ptr32 Void
// +0x018 Body             : _QUAD
// PspCidTable��¼��Objectλ��Body��, ���� -0x10����_OBJECT_TYPEָ��
BOOLEAN
IsProcessObject(PVOID Object)
{
    PVOID type;
    
    if(MmIsAddressValid(Object) == FALSE)
        return FALSE;
    if(MmIsAddressValid((PUCHAR)Object - 0x10) == FALSE)
        return FALSE; 

    type = *(PVOID*)((PUCHAR)Object - 0x10);
    if(type == *PsProcessType)
        return TRUE;

    return FALSE;
}


BOOLEAN
IsThreadObject(PVOID Object)
{
    PVOID type;

    if(MmIsAddressValid(Object) == FALSE)
        return FALSE;
    if(MmIsAddressValid((PUCHAR)Object - 0x10) == FALSE)
        return FALSE; 

    type = *(PVOID*)((PUCHAR)Object - 0x10);
    if(type == *PsThreadType)
        return TRUE;

    return FALSE;
}
// kd> dt _HANDLE_TABLE_ENTRY
// nt!_HANDLE_TABLE_ENTRY
// +0x000 Object           : Ptr32 Void
// +0x000 ObAttributes     : Uint4B
// +0x000 InfoTable        : Ptr32 _HANDLE_TABLE_ENTRY_INFO
// +0x000 Value            : Uint4B
// +0x004 GrantedAccess    : Uint4B
// +0x004 GrantedAccessIndex : Uint2B
// +0x006 CreatorBackTraceIndex : Uint2B
// +0x004 NextFreeTableEntry : Int4B
// HandleTableEntry �ṹ����, ��һ����Ա����Object
// Object & 0xFFFFFFF8 | 0x8FFFFFFF���������Ķ���ָ��
BOOLEAN
EnumHandleCallbackProcess(PVOID HandleTableEntry,
                         HANDLE Handle,
                         PVOID EnumParam
                         )
{
    ULONG Object;
    PEnumContext context;

    context = (PEnumContext)EnumParam;

    //�õ�����ָ��
    if(MmIsAddressValid(HandleTableEntry) == FALSE)
        return TRUE;
    Object = *(ULONG*)HandleTableEntry;
    Object = (Object & 0xFFFFFFF8) | 0x8FFFFFFF;

    if(IsProcessObject(Object))
    {
        if((context->showDeleting || IsProcessDeleting(Object)) &&
           HashTableMark(context->hashT, Object))
        {
            context->objIdT->Entry[context->objIdT->Count].Object = (PVOID)Object;
            context->objIdT->Entry[context->objIdT->Count].UniqueId = (ULONG)Handle;
            context->objIdT->Count++;
        }
    }
    else if(IsThreadObject(Object))
    {
        
    }

    return TRUE;
}

//ö��ϵͳ�еĽ���
//�ɹ�����ObjectIdTable�ṹָ��
//ʧ�ܷ���NULL
PObjectIdTable
ProcessEnum(BOOLEAN ShowDeleting)
{
    EnumContext context;

    //���벢��ʼ��������
    context.showDeleting = ShowDeleting;
    context.hashT = ExAllocatePoolWithTag(NonPagedPool,
                                          sizeof(HashTable),
                                          'hata');
    if(context.hashT == NULL)
    {
        KdPrint(("can not allocate hash table in ProcessEnum.\n"));
        return NULL;
    }

    context.objIdT = ExAllocatePoolWithTag(NonPagedPool,
                                           sizeof(ObjectIdTable),
                                           'oita');
    if(context.objIdT == NULL)
    {
        KdPrint(("can not allocate object id table in ProcessEnum.\n"));
        ExFreePool(context.hashT);
        return NULL;
    }

    HashTableInitialize(&context.hashT);
    RtlZeroMemory(&context.objIdT, sizeof(ObjectIdTable));

    //

    ExFreePool(context.hashT);
    return context.objIdT;
}

//�ͷ�ObjectIdTable��ռ�õĿռ�
inline VOID
ProcessFreeObjIdTable(PObjectIdTable ObjectIdT)
{
    if(ObjectIdT != NULL)
        ExFreePool(ObjectIdT);
}