#pragma once

#ifndef CRARKKRNL_EXPORTS
#define CRARKAPI extern "C" __declspec(dllimport)
#else
#define CRARKAPI extern "C" __declspec(dllexport)
#endif

//初始化dll文件
//包括释放、加载、初始化、链接驱动程序
CRARKAPI BOOL CrInitialize();

//关闭连接、卸载驱动、释放资源等
CRARKAPI void CrUninitialize();