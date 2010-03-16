#pragma once

BOOLEAN 
IsProcessDeleting(PVOID EProcess);

BOOLEAN
IsProcessObject(PVOID Object);

BOOLEAN
IsThreadObject(PVOID Object);

//将驱动路径转化为dos路径
//DevicePath 为 \Device\HardVolumn.....
//Allocate   是否申请空间
//PoolType   Allocate为TRUE时申请的类型
//OutBuffer  输出转换后的字符串
BOOLEAN
ConvertDevicePathToDosPath(
                           IN PUNICODE_STRING DevicePath,
                           IN BOOLEAN Allocate,
                           IN POOL_TYPE PoolType,
                           OUT PUNICODE_STRING OutBuffer
                           );

//挂钩Function函数至FakeFunction
//JmpBuffer是FakeFunction跳转回Function的跳转缓冲区
BOOLEAN
HookProcess(PVOID Function, PVOID FakeFunction, PUCHAR JmpBuffer);

//打开或关闭内存写保护
VOID
EnableWritePretect(BOOLEAN Enable);