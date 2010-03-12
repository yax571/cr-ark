#pragma once

#define OBJECT_ID_TABLE_LENGTH 65535

typedef struct _ObjectIdPair{
    PVOID Object;
    ULONG UniqueId;
}ObjectIdPair, *PObjectIdPair;

typedef struct _ObjectIdTable{
    USHORT Count;
    ObjectIdPair Entry[OBJECT_ID_TABLE_LENGTH];
}ObjectIdTable, *PObjectIdTable;

//ö��ϵͳ�еĽ���
//�ɹ�����ObjectIdTable�ṹָ��
//ʧ�ܷ���NULL
PObjectIdTable
ProcessEnum(BOOLEAN ShowDeleting);


PObjectIdTable
ThreadEnum(PEPROCESS Process);


//�ͷ�ObjectIdTable��ռ�õĿռ�
VOID
FreeObjIdTable(PObjectIdTable ObjectIdT);