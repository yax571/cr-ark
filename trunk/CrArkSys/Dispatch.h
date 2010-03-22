#pragma once

//��ʼ��APC
//InputBuffer[0] == ThreadHandle
NTSTATUS
DispatchSpecialInitialize(PVOID InputBuffer, ULONG InputLength,
                          PVOID OutputBuffer, ULONG OutputLength,
                          PULONG Information);

//ö�ٽ���
//InputBuffer[0] == �Ƿ���ʾDeleting����
//OutputBuffer ObjectIdTable����
NTSTATUS
DispatchProcessEnum(PVOID InputBuffer, ULONG InputLength,
                    PVOID OutputBuffer, ULONG OutputLength,
                    PULONG Information);

//ö�ٽ���
//InputBuffer[0] == �Ƿ���ʾEPROCESSָ��
//OutputBuffer ObjectIdTable����
NTSTATUS
DispatchThreadEnum(PVOID InputBuffer, ULONG InputLength,
                   PVOID OutputBuffer, ULONG OutputLength,
                   PULONG Information);

//\��ȡ��������
//InputBuffer[0] == PEPROCESSָ��
//OutputBuffer ProcessNameInfo����
NTSTATUS
DispatchQueryProcessName(PVOID InputBuffer, ULONG InputLength,
                         PVOID OutputBuffer, ULONG OutputLength,
                         PULONG Information);

//��ȡ������Ϣ
//InputBuffer[0] == PEPROCESSָ��
//OutputBuffer ProcessInfo����
NTSTATUS
DispatchQueryProcessInfo(PVOID InputBuffer, ULONG InputLength,
                         PVOID OutputBuffer, ULONG OutputLength,
                         PULONG Information);

//��ȡ�߳���Ϣ
//InputBuffer[0] == PETHREADָ��
//OutputBuffer threadInfo����
NTSTATUS
DispatchQueryThreadInfo(PVOID InputBuffer, ULONG InputLength,
                        PVOID OutputBuffer, ULONG OutputLength,
                        PULONG Information);

//��ȡ���̵�ģ���б�
//InputBuffer[0] == PEPROCESSָ��
//OutputBuffer ProcessModuleList����
NTSTATUS
DispatchQueryProcessModuleList(PVOID InputBuffer, ULONG InputLength,
                               PVOID OutputBuffer, ULONG OutputLength,
                               PULONG Information);

//��ȡ���̵�ģ����Ϣ
//InputBuffer[0] == PEPROCESSָ��
//InputBuffer[1] == LdrDataָ��
//OutputBuffer ModuleInfo����
NTSTATUS
DispatchQueryProcessModuleInfo(PVOID InputBuffer, ULONG InputLength,
                               PVOID OutputBuffer, ULONG OutputLength,
                               PULONG Information);

//�����߳�
//InputBuffer[0] == PETHREADָ��
//InputBuffer[1] == ExitStatus
//InputBuffer[2] == ForceExit
NTSTATUS
DispatchTerminateThread(PVOID InputBuffer, ULONG InputLength,
                        PVOID OutputBuffer, ULONG OutputLength,
                        PULONG Information);

//��������
//InputBuffer[0] == PPROCESSָ��
//InputBuffer[1] == ExitStatus
//InputBuffer[2] == ForceExit
NTSTATUS
DispatchTerminateProcess(PVOID InputBuffer, ULONG InputLength,
                         PVOID OutputBuffer, ULONG OutputLength,
                         PULONG Information);

//ж��Process�����е�ָ��ģ��
//InputBuffer[0] == Process      PEPROCESSָ��
//InputBuffer[1] == BaseAddress  ģ����ʼ��ַ
NTSTATUS
DispatchUnmapProcessModule(PVOID InputBuffer, ULONG InputLength,
                           PVOID OutputBuffer, ULONG OutputLength,
                           PULONG Information);