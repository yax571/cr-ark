#pragma once

#ifndef CRARKKRNL_EXPORTS
#define CRARKAPI extern "C" __declspec(dllimport)
#else
#define CRARKAPI extern "C" __declspec(dllexport)
#endif

//��ʼ��dll�ļ�
//�����ͷš����ء���ʼ����������������
CRARKAPI BOOL CrInitialize();

//�ر����ӡ�ж���������ͷ���Դ��
CRARKAPI void CrUninitialize();