#include <ntddk.h>
#include "HashTable.h"

//初始化哈希表
VOID
HashTableInitialize(PHashTable T)
{
    USHORT i;

    RtlZeroMemory(T, sizeof(HashTable));
    for(i = 0; i <= HASH_TABLE_LENGTH - 2; i++)
    {
        T->Pool[i].Next = i + 1;
    }
}


//内部使用的函数, 遍历哈希表
//T     哈希表指针
//Data  数据
//Delete 是否删除表项
//返回是否找到
BOOLEAN
HashTableScanData(PHashTable T, ULONG Data, BOOLEAN Delete)
{
    USHORT hash;
    USHORT pointer, next;

    hash = (USHORT)(Data % HASH_TABLE_LENGTH);
    pointer = T->FirstEntry[hash];

    //表项为空
    if(pointer == 0)
        return FALSE;

    //第一项要特殊处理
    if(T->Pool[pointer].Data == Data)
    {
        if(Delete)
        {
            T->FirstEntry[pointer] = T->Pool[pointer].Next;
            T->Pool[pointer].Next = T->Pool[0].Next;
            T->Pool[0].Next = pointer;
            T->Count--;
        }
        return TRUE;
    }

    //中间项及最后一项的处理
    next = T->Pool[pointer].Next;
    while(next != 0)
    {
        if(T->Pool[next].Data == Data)
        {
            if(Delete)
            {
                T->Pool[pointer].Next = T->Pool[next].Next;
                T->Pool[next].Next = T->Pool[0].Next;
                T->Pool[0].Next = next;
                T->Count--;
            }
            return TRUE;
        }
        pointer = next;
        next = T->Pool[next].Next;
    }

    return FALSE;
}

//检测哈希表中是否有Data项
BOOLEAN
HashTableCheck(PHashTable T, ULONG Data)
{
    return HashTableScanData(T, Data, FALSE);
}

//删除哈希表中的Data项
BOOLEAN
HashTableDelete(PHashTable T, ULONG Data)
{
    return HashTableScanData(T, Data, TRUE);
}

//将Data插入到哈希表T中
//成功返回TRUE, 失败返回FALSE
BOOLEAN
HashTableMark(PHashTable T, ULONG Data)
{
    USHORT hash;
    USHORT pointer;

    //已经有Data项了
    if(HashTableScanData(T, Data, FALSE))
        return FALSE;

    hash = (USHORT)(Data % HASH_TABLE_LENGTH);
    pointer = T->Pool[0].Next;

    //没有空表项了
    if(pointer == 0)
        return FALSE;
    
    //插入哈希表
    T->Pool[0].Next = T->Pool[pointer].Next;
    T->Pool[pointer].Next = T->FirstEntry[hash];
    T->Pool[pointer].Data = Data;
    T->FirstEntry[hash] = pointer;
    T->Count++;

    return TRUE;
}