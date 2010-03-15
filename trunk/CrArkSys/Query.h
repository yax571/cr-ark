#pragma once

#define FULL_PATH_LENGTH 266

typedef struct _ProcessNameInfo {
    char ImageName[17];
    char FullPath[FULL_PATH_LENGTH];
}ProcessNameInfo, *PProcessNameInfo;

//根据EProcess结构获得镜像名称和路径
//失败返回NULL
PProcessNameInfo
QueryProcessName(PEPROCESS Process);