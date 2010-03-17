#include "ntifs.h"
#include "Dispatch.h"
#include "Enviroment.h"

//初始化APC
//InputBuffer[0] == ThreadHandle
NTSTATUS
DispatchSpecialInitialize(PVOID InputBuffer, ULONG InputLength,
                          PVOID OutputBuffer, ULONG OutputLength,
                          PULONG Information)
{
    HANDLE threadHandle;
    NTSTATUS status;

    //验证
    if(InputBuffer == NULL ||
       InputLength != sizeof(HANDLE))
       return STATUS_INVALID_PARAMETER;
    *Information = 0;

    status = STATUS_SUCCESS;
    try{
        ProbeForRead(InputBuffer, 4, 1);
        threadHandle = *(HANDLE*)(InputBuffer);
    }except(EXCEPTION_CONTINUE_EXECUTION){
        status = STATUS_ACCESS_VIOLATION;
    }

    if(!NT_SUCCESS(status))
        return status;

    if(!EnviromentSpecialInitialize(threadHandle, NULL, TRUE))
        return STATUS_UNSUCCESSFUL;

    return STATUS_SUCCESS;
}