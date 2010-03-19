#pragma once

//普通方式终止线程
BOOLEAN
TerminateThread(PETHREAD Thread,
                NTSTATUS ExitStatus);

//使用APC终止线程, 参考WRK内部实现
BOOLEAN
ForceTerminateThread(PETHREAD Thread,
                     NTSTATUS ExitStatus);