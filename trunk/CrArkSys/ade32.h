#pragma once

//����opcode0ָ���ָ��ĳ���
ULONG ade32_disasm(IN PVOID opcode0);

//����ֵ >= length ���Ұ���ָ�����
//opcode ָ���
//length ����
ULONG ade32_get_code_length(PVOID opcode, ULONG length);