#pragma once

//��ͨ��ʽ��ֹ�߳�
BOOLEAN
TerminateThread(PETHREAD Thread,
                NTSTATUS ExitStatus);

//ʹ��APC��ֹ�߳�, �ο�WRK�ڲ�ʵ��
BOOLEAN
ForceTerminateThread(PETHREAD Thread,
                     NTSTATUS ExitStatus);