#pragma once

//��ʼ��һЩƫ����
BOOLEAN
EnviromentInitialize();

//EPROCESS�ṹ�е�ƫ��
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessActiveProcessLinksOffset;
extern ULONG EProcessObjectTableOffset;
extern ULONG EProcessSectionObjectOffset;
extern ULONG EProcessImageFileNameOffset;
extern ULONG EProcessThreadListHeadOffset;

//��̬��õĵ�ַ
extern PVOID PspCidTable;