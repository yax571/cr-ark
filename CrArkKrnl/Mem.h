#pragma once

//读内核内存
DWORD WINAPI ReadKernelMem(PVOID BaseAddress, PVOID DestAddress, DWORD Length);

//写内核内存
DWORD WINAPI WriteKernelMem(PVOID SourceAddress, PVOID DestAddress, DWORD Length);