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

//初始化哈希表
VOID
HashTableInitialize(PHashTable T);

//检测哈希表中是否有Data项
BOOLEAN
HashTableCheck(PHashTable T, ULONG Data);

//删除哈希表中的Data项
BOOLEAN
HashTableDelete(PHashTable T, ULONG Data);

//将Data插入到哈希表T中
//成功返回TRUE, 失败返回FALSE
BOOLEAN
HashTableMark(PHashTable T, ULONG Data);