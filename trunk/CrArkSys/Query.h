#pragma once

#define FULL_PATH_LENGTH 266

typedef struct _ProcessNameInfo {
    char ImageName[17];
    char FullPath[FULL_PATH_LENGTH];
}ProcessNameInfo, *PProcessNameInfo;

//����EProcess�ṹ��þ������ƺ�·��
//ʧ�ܷ���NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process);