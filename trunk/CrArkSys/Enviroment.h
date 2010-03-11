#pragma once

//初始化一些偏移量
BOOLEAN
EnviromentInitialize();

//EPROCESS结构中的偏移
extern ULONG EProcessUniqueProcessIdOffset;
extern ULONG EProcessActiveProcessLinksOffset;
extern ULONG EProcessObjectTableOffset;
extern ULONG EProcessSectionObjectOffset;
extern ULONG EProcessImageFileNameOffset;
extern ULONG EProcessThreadListHeadOffset;

//动态获得的地址
extern PVOID PspCidTable;