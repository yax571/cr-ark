#include <ntddk.h>
#include "HashTable.h"

//��ʼ����ϣ��
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


//�ڲ�ʹ�õĺ���, ������ϣ��
//T     ��ϣ��ָ��
//Data  ����
//Delete �Ƿ�ɾ������
//�����Ƿ��ҵ�
BOOLEAN
HashTableScanData(PHashTable T, ULONG Data, BOOLEAN Delete)
{
    USHORT hash;
    USHORT pointer, next;

    hash = (USHORT)(Data % HASH_TABLE_LENGTH);
    pointer = T->FirstEntry[hash];

    //����Ϊ��
    if(pointer == 0)
        return FALSE;

    //��һ��Ҫ���⴦��
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

    //�м�����һ��Ĵ���
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

//����ϣ�����Ƿ���Data��
BOOLEAN
HashTableCheck(PHashTable T, ULONG Data)
{
    return HashTableScanData(T, Data, FALSE);
}

//ɾ����ϣ���е�Data��
BOOLEAN
HashTableDelete(PHashTable T, ULONG Data)
{
    return HashTableScanData(T, Data, TRUE);
}

//��Data���뵽��ϣ��T��
//�ɹ�����TRUE, ʧ�ܷ���FALSE
BOOLEAN
HashTableMark(PHashTable T, ULONG Data)
{
    USHORT hash;
    USHORT pointer;

    //�Ѿ���Data����
    if(HashTableScanData(T, Data, FALSE))
        return FALSE;

    hash = (USHORT)(Data % HASH_TABLE_LENGTH);
    pointer = T->Pool[0].Next;

    //û�пձ�����
    if(pointer == 0)
        return FALSE;
    
    //�����ϣ��
    T->Pool[0].Next = T->Pool[pointer].Next;
    T->Pool[pointer].Next = T->FirstEntry[hash];
    T->Pool[pointer].Data = Data;
    T->FirstEntry[hash] = pointer;
    T->Count++;

    return TRUE;
}