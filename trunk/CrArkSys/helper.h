#pragma once

BOOLEAN 
IsProcessDeleting(PVOID EProcess);

BOOLEAN
IsProcessObject(PVOID Object);

BOOLEAN
IsThreadObject(PVOID Object);

//������·��ת��Ϊdos·��
//DevicePath Ϊ \Device\HardVolumn.....
//Allocate   �Ƿ�����ռ�
//PoolType   AllocateΪTRUEʱ���������
//OutBuffer  ���ת������ַ���
BOOLEAN
ConvertDevicePathToDosPath(
                           IN PUNICODE_STRING DevicePath,
                           IN BOOLEAN Allocate,
                           IN POOL_TYPE PoolType,
                           OUT PUNICODE_STRING OutBuffer
                           );

//�ҹ�Function������FakeFunction
//JmpBuffer��FakeFunction��ת��Function����ת������
BOOLEAN
HookProcess(PVOID Function, PVOID FakeFunction, PUCHAR JmpBuffer);

//�򿪻�ر��ڴ�д����
VOID
EnableWritePretect(BOOLEAN Enable);