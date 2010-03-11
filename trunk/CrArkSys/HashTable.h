#pragma once

#define HASH_TABLE_LENGTH 65535

typedef struct _HashTableEntry{
    ULONG Data;
    USHORT Next;
}HashTableEntry, *PHashTableEntry;

typedef struct _HashTable{
    ULONG Count;
    USHORT FirstEntry[HASH_TABLE_LENGTH];
    HashTableEntry Pool[HASH_TABLE_LENGTH]; 
}HashTable, *PHashTable;

//��ʼ����ϣ��
VOID
HashTableInitialize(PHashTable T);

//����ϣ�����Ƿ���Data��
BOOLEAN
HashTableCheck(PHashTable T, ULONG Data);

//ɾ����ϣ���е�Data��
BOOLEAN
HashTableDelete(PHashTable T, ULONG Data);

//��Data���뵽��ϣ��T��
//�ɹ�����TRUE, ʧ�ܷ���FALSE
BOOLEAN
HashTableMark(PHashTable T, ULONG Data);