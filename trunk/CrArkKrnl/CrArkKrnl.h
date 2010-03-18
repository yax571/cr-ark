#pragma once

#ifndef CRARKKRNL_EXPORTS
#define CRARKAPI extern "C" __declspec(dllimport)
#else
#define CRARKAPI extern "C" __declspec(dllexport)
#endif

#define OBJECT_ID_TABLE_LENGTH 65535

typedef struct _ObjectIdPair{
    PVOID Object;
    DWORD UniqueId;
}ObjectIdPair, *PObjectIdPair;

typedef struct _ObjectIdTable{
    WORD Count;
    ObjectIdPair Entry[OBJECT_ID_TABLE_LENGTH];
}ObjectIdTable, *PObjectIdTable;

//初始化dll文件
//包括释放、加载、初始化、链接驱动程序
CRARKAPI BOOL CrInitialize();

//关闭连接、卸载驱动、释放资源等
CRARKAPI void CrUninitialize();

//枚举进程
CRARKAPI PObjectIdTable CrProcessEnum(BOOL ShowDeleting);

//枚举线程
CRARKAPI PObjectIdTable CrThreadEnum(PVOID Process);