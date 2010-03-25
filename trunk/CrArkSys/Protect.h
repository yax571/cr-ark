#pragma once


//初始化保护模块
BOOLEAN
ProtectInitialize();

//清除保护模块的资源
VOID
ProtectCleanup();

//加入保护的进程或线程
//Remove == TRUE 删除
VOID
ProtectAddObject(IN PVOID Object,
                 IN BOOLEAN Remove);