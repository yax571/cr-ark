#include <ntddk.h>
#include "ProcEnum.h"
#include "HashTable.h"
#include "Undocument.h"
#include "Enviroment.h"
#include "helper.h"

typedef struct _EnumContext{
    union{
        PEPROCESS process;      //ö���߳�ʱʹ��
        BOOLEAN showDeleting;   //ö�ٽ���ʱʹ��
    };
    PHashTable hashT;
    PObjectIdTable objIdT;
}EnumContext, *PEnumContext;

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
    ULONG Object, Process, ProcessId;
    PEnumContext context;

    context = (PEnumContext)EnumParam;

    //�õ�����ָ��
    if(MmIsAddressValid(HandleTableEntry) == FALSE)
        return FALSE;
    Object = *(ULONG*)HandleTableEntry;
    Object = (Object & 0xFFFFFFF8) | 0x80000000;

    if(IsProcessObject((PVOID)Object))
    {
        if((context->showDeleting || !IsProcessDeleting((PVOID)Object)) &&
           HashTableMark(context->hashT, Object))
        {
            context->objIdT->Entry[context->objIdT->Count].Object = (PVOID)Object;
            context->objIdT->Entry[context->objIdT->Count].UniqueId = (ULONG)Handle;
            context->objIdT->Count++;
        }
    }
    else if(IsThreadObject((PVOID)Object))
    {
        if(MmIsAddressValid((PUCHAR)Object + EThreadApcStateProcessOffset) == FALSE)
            return FALSE;
        if(MmIsAddressValid((PUCHAR)Object + EThreadCidOffset) == FALSE)
            return FALSE;

        Process = *(PULONG)((PUCHAR)Object + EThreadApcStateProcessOffset);
        ProcessId = *(PULONG)((PUCHAR)Object + EThreadCidOffset);

        if(!IsProcessObject((PVOID)Process))
            return FALSE;

        if((context->showDeleting || !IsProcessDeleting((PVOID)Process)) &&
            HashTableMark(context->hashT, Process))
        {
            context->objIdT->Entry[context->objIdT->Count].Object = (PVOID)Process;
            context->objIdT->Entry[context->objIdT->Count].UniqueId = ProcessId;
            context->objIdT->Count++;
        }
    }

    return FALSE;
}


BOOLEAN
EnumHandleCallbackThread(PVOID HandleTableEntry,
                          HANDLE Handle,
                          PVOID EnumParam
                          )
{
    PEnumContext context;
    PVOID object;

    context = (PEnumContext)EnumParam;

    //�õ�����ָ��
    if(!MmIsAddressValid(HandleTableEntry))
        return FALSE;

    object = *(PVOID*)HandleTableEntry;
    object = (PVOID)(((ULONG)object & 0xFFFFFFF8) | 0x80000000);

    if(IsThreadObject(object) && 
       context->process == *(PEPROCESS*)((ULONG)object + EThreadApcStateProcessOffset) &&
       HashTableMark(context->hashT, (ULONG)object))
    {
        context->objIdT->Entry[context->objIdT->Count].Object = object;
        context->objIdT->Entry[context->objIdT->Count].UniqueId = (ULONG)Handle;
        context->objIdT->Count++;
    }

    return FALSE;
}

VOID
EnumThreadByThreadList(PEnumContext Context)
{
    PLIST_ENTRY start, current;
    KIRQL oldIrql;
    PVOID thread;
    ULONG uniqueId;

    if(!IsProcessObject(Context->process))
        return;

    oldIrql = KeRaiseIrqlToDpcLevel();

    start = (PLIST_ENTRY)((ULONG)Context->process + EProcessThreadListHeadOffset);
    current = start->Flink;

    while(current != start)
    {
        thread = (PUCHAR)current - EThreadThreadListEntryOffset;
        uniqueId = *(PULONG)((PUCHAR)thread + EThreadCidOffset + 4);
        if(HashTableMark(Context->hashT, (ULONG)thread))
        {
            Context->objIdT->Entry[Context->objIdT->Count].Object = thread;
            Context->objIdT->Entry[Context->objIdT->Count].UniqueId = uniqueId;
            Context->objIdT->Count++;
        }
        current = current->Flink;
    }

    KeLowerIrql(oldIrql);
}

VOID
EnumProcessByActiveLink(PEnumContext Context)
{
    PLIST_ENTRY start, current;
    PVOID process;
    ULONG uniqueId;
    KIRQL oldIrql;

    oldIrql = KeRaiseIrqlToDpcLevel();

    start = ActiveProcessLinksHead;
    current = start->Flink;

    while(current != start)
    {
        process = (PVOID)((ULONG)current - EProcessActiveProcessLinksOffset);
        uniqueId = *(PULONG)((ULONG)process + EProcessUniqueProcessIdOffset);
        if((Context->showDeleting || !IsProcessDeleting(process)) &&
            HashTableMark(Context->hashT, (ULONG)process))
        {
            Context->objIdT->Entry[Context->objIdT->Count].Object = process;
            Context->objIdT->Entry[Context->objIdT->Count].UniqueId = uniqueId;
            Context->objIdT->Count++;
        }
        current = current->Flink;
    }

    KeLowerIrql(oldIrql);
}

VOID
EnumProcessByHandleTable(PEnumContext Context)
{
    PLIST_ENTRY start, current;
    KIRQL oldIrql;
    PVOID process;
    ULONG uniqueId;
    PUCHAR handleTable;

    oldIrql = KeRaiseIrqlToDpcLevel();

    start = HandleTableListHead;
    current = start->Flink;

    while(current != start)
    {
        handleTable = (PUCHAR)current - HandleTableHandleTableListOffset;
        if(MmIsAddressValid(handleTable + HandleTableQuotaProcessOffset) == FALSE ||
            MmIsAddressValid(handleTable + HandleTableUniqueProcessIdOffset) == FALSE)
        {
            current = current->Flink;
            continue;
        }

        process = *(PVOID*)(handleTable + HandleTableQuotaProcessOffset);
        uniqueId = *(PULONG)(handleTable + HandleTableUniqueProcessIdOffset);

        if( process != NULL &&
            (Context->showDeleting || !IsProcessDeleting(process)) &&
            HashTableMark(Context->hashT, (ULONG)process))
        {
            Context->objIdT->Entry[Context->objIdT->Count].Object = process;
            Context->objIdT->Entry[Context->objIdT->Count].UniqueId = uniqueId;
            Context->objIdT->Count++;
        }

        current = current->Flink;
    }

    KeLowerIrql(oldIrql);
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

    HashTableInitialize(context.hashT);
    RtlZeroMemory(context.objIdT, sizeof(ObjectIdTable));

    //����PspCidTable��ȡ����
    ExEnumHandleTable(PspCidTable,
                      EnumHandleCallbackProcess,
                      &context,
                      NULL);

    //����ActiveProcessLink��ȡ����
    EnumProcessByActiveLink(&context);

    //����������ȡ����
    EnumProcessByHandleTable(&context);

    ExFreePool(context.hashT);
    return context.objIdT;
}


PObjectIdTable
ThreadEnum(PEPROCESS Process)
{
    EnumContext context;

    //����֤�Ƿ���Process�ṹ
    if(IsProcessObject(Process) == FALSE)
        return NULL;

    //��ʼ��������
    context.process = Process;
    context.hashT = ExAllocatePoolWithTag(NonPagedPool,
                                          sizeof(HashTable),
                                          'hata');
    if(context.hashT == NULL)
        return NULL;

    context.objIdT = ExAllocatePoolWithTag(NonPagedPool,
                                           sizeof(ObjectIdTable),
                                           'oita');
    if(context.objIdT == NULL)
    {
        ExFreePool(context.hashT);
        return NULL;
    }

    HashTableInitialize(context.hashT);
    RtlZeroMemory(context.objIdT, sizeof(ObjectIdTable));

    //����PspCidTable����߳���Ϣ
    ExEnumHandleTable(PspCidTable,
                      EnumHandleCallbackThread,
                      &context,
                      NULL);

    //����ThreadList����߳���Ϣ
    EnumThreadByThreadList(&context);

    ExFreePool(context.hashT);
    return context.objIdT;
}


//�ͷ�ObjectIdTable��ռ�õĿռ�
VOID
FreeObjIdTable(PObjectIdTable ObjectIdT)
{
    if(ObjectIdT != NULL)
        ExFreePool(ObjectIdT);
}