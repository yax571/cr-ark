#pragma once

//普通方式终止线程
BOOLEAN
TerminateThread(PETHREAD Thread,
                NTSTATUS ExitStatus);

//使用APC终止线程, 参考WRK内部实现
BOOLEAN
ForceTerminateThread(PETHREAD Thread,
                     NTSTATUS ExitStatus);

//普通方式结束进程
BOOLEAN
TerminateProcess(PEPROCESS Process, 
                 NTSTATUS uExitCode );

//使用APC结束进程
BOOLEAN
ForceTerminateProcess(PEPROCESS Process,
                      NTSTATUS ExitStatus);

//卸载Process进程中的以BaseAddress为起始地址的模块
BOOLEAN
UnmapProcessModule(PEPROCESS Process, PVOID BaseAddress);