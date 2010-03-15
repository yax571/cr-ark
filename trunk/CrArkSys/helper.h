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