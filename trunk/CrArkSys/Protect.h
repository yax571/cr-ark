#pragma once


//��ʼ������ģ��
BOOLEAN
ProtectInitialize();

//�������ģ�����Դ
VOID
ProtectCleanup();

//���뱣���Ľ��̻��߳�
//Remove == TRUE ɾ��
VOID
ProtectAddObject(IN PVOID Object,
                 IN BOOLEAN Remove);