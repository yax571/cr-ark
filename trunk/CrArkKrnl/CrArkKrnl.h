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

//��ʼ��dll�ļ�
//�����ͷš����ء���ʼ����������������
CRARKAPI BOOL CrInitialize();

//�ر����ӡ�ж���������ͷ���Դ��
CRARKAPI void CrUninitialize();

//ö�ٽ���
CRARKAPI PObjectIdTable CrProcessEnum(BOOL ShowDeleting);

//ö���߳�
CRARKAPI PObjectIdTable CrThreadEnum(PVOID Process);