#pragma once

//���ں��ڴ�
DWORD WINAPI ReadKernelMem(PVOID BaseAddress, PVOID DestAddress, DWORD Length);

//д�ں��ڴ�
DWORD WINAPI WriteKernelMem(PVOID SourceAddress, PVOID DestAddress, DWORD Length);