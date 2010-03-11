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
ProcessEnum();

//�ͷ�ObjectIdTable��ռ�õĿռ�
VOID
ProcessFreeObjIdTable(PObjectIdTable ObjectIdT);