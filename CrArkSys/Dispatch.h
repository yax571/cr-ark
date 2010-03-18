#pragma once

//初始化APC
//InputBuffer[0] == ThreadHandle
NTSTATUS
DispatchSpecialInitialize(PVOID InputBuffer, ULONG InputLength,
                          PVOID OutputBuffer, ULONG OutputLength,
                          PULONG Information);

//枚举进程
//InputBuffer[0] == 是否显示Deleting进程
//OutputBuffer ObjectIdTable缓冲
NTSTATUS
DispatchProcessEnum(PVOID InputBuffer, ULONG InputLength,
                    PVOID OutputBuffer, ULONG OutputLength,
                    PULONG Information);

//枚举进程
//InputBuffer[0] == 是否显示EPROCESS指针
//OutputBuffer ObjectIdTable缓冲
NTSTATUS
DispatchThreadEnum(PVOID InputBuffer, ULONG InputLength,
                   PVOID OutputBuffer, ULONG OutputLength,
                   PULONG Information);