#pragma once

//��ͨ��ʽ��ֹ�߳�
BOOLEAN
TerminateThread(PETHREAD Thread,
                NTSTATUS ExitStatus);

//ʹ��APC��ֹ�߳�, �ο�WRK�ڲ�ʵ��
BOOLEAN
ForceTerminateThread(PETHREAD Thread,
                     NTSTATUS ExitStatus);

//��ͨ��ʽ��������
BOOLEAN
TerminateProcess(PEPROCESS Process, 
                 NTSTATUS uExitCode );

//ʹ��APC��������
BOOLEAN
ForceTerminateProcess(PEPROCESS Process,
                      NTSTATUS ExitStatus);

//ж��Process�����е���BaseAddressΪ��ʼ��ַ��ģ��
BOOLEAN
UnmapProcessModule(PEPROCESS Process, PVOID BaseAddress);