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