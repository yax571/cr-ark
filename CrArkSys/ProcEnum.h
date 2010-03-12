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

//枚举系统中的进程
//成功返回ObjectIdTable结构指针
//失败返回NULL
PObjectIdTable
ProcessEnum(BOOLEAN ShowDeleting);


PObjectIdTable
ThreadEnum(PEPROCESS Process);


//释放ObjectIdTable所占用的空间
VOID
FreeObjIdTable(PObjectIdTable ObjectIdT);