#include <ntifs.h>
#include <string.h>
#include "Undocument.h"
#include "ade32.h"


BOOLEAN 
IsProcessDeleting(PVOID EProcess)
{
    ULONG signalState;

    if(MmIsAddressValid((PUCHAR)EProcess + 0x04) == FALSE)
        return TRUE;

    signalState = *(PULONG)((PUCHAR)EProcess + 0x04);
    return signalState > 0;
}

//kd> dt _OBJECT_HEADER
// nt!_OBJECT_HEADER
// +0x000 PointerCount     : Int4B
// +0x004 HandleCount      : Int4B
// +0x004 NextToFree       : Ptr32 Void
// +0x008 Type             : Ptr32 _OBJECT_TYPE
// +0x00c NameInfoOffset   : UChar
// +0x00d HandleInfoOffset : UChar
// +0x00e QuotaInfoOffset  : UChar
// +0x00f Flags            : UChar
// +0x010 ObjectCreateInfo : Ptr32 _OBJECT_CREATE_INFORMATION
// +0x010 QuotaBlockCharged : Ptr32 Void
// +0x014 SecurityDescriptor : Ptr32 Void
// +0x018 Body             : _QUAD
// PspCidTable记录的Object位于Body处, 所以 -0x10就是_OBJECT_TYPE指针
BOOLEAN
IsProcessObject(PVOID Object)
{
    PVOID type;

    if(MmIsAddressValid(Object) == FALSE)
        return FALSE;
    if(MmIsAddressValid((PUCHAR)Object - 0x10) == FALSE)
        return FALSE; 

    type = *(PVOID*)((PUCHAR)Object - 0x10);
    if(type == *PsProcessType)
        return TRUE;

    return FALSE;
}


BOOLEAN
IsThreadObject(PVOID Object)
{
    PVOID type;

    if(MmIsAddressValid(Object) == FALSE)
        return FALSE;
    if(MmIsAddressValid((PUCHAR)Object - 0x10) == FALSE)
        return FALSE; 

    type = *(PVOID*)((PUCHAR)Object - 0x10);
    if(type == *PsThreadType)
        return TRUE;

    return FALSE;
}

//将驱动路径转化为dos路径
//DevicePath 为 \Device\HardVolumn.....
//Allocate   是否申请空间
//PoolType   Allocate为TRUE时申请的类型
//OutBuffer  输出转换后的字符串
BOOLEAN
ConvertDevicePathToDosPath(
                           IN PUNICODE_STRING DevicePath,
                           IN BOOLEAN Allocate,
                           IN POOL_TYPE PoolType,
                           OUT PUNICODE_STRING OutBuffer
                           )
{
    UNICODE_STRING devName;
    UNICODE_STRING symbolName;
    WCHAR symbolNameBuffer[] = L"\\??\\X:";
    OBJECT_ATTRIBUTES oas;
    HANDLE symbolLinkHandle;
    NTSTATUS status;
    WCHAR i, *pwchar, *pwchar2;
    BOOLEAN ret, found;
    ULONG retLength, length;

    ret = FALSE;
    //内核栈比较小, 把设备名放在堆中
    devName.Buffer = ExAllocatePoolWithTag(PagedPool,
                                           128,
                                           'devn');
    devName.Length = 0;
    devName.MaximumLength = 128;

    //遍历所有磁盘符号链接
    found = FALSE;
    RtlInitUnicodeString(&symbolName, symbolNameBuffer);
    for(i = L'A'; i <= L'Z'; i++)
    {
        symbolName.Buffer[4] = i;
        InitializeObjectAttributes(&oas,
                                   &symbolName,
                                   OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                   NULL, NULL);
        status = ZwOpenSymbolicLinkObject(&symbolLinkHandle,
                                          GENERIC_READ,
                                          &oas);
        if(!NT_SUCCESS(status))
            continue;

        status = ZwQuerySymbolicLinkObject(symbolLinkHandle,
                                            &devName,
                                            &retLength);
        ZwClose(symbolLinkHandle);
        //一般磁盘符号链接的长度都可以在devName中放下
        if(!NT_SUCCESS(status))
            continue;

        if(_wcsnicmp(devName.Buffer, DevicePath->Buffer, devName.Length / sizeof(WCHAR)) == 0)
        {
            found = TRUE;
            break;
        }
    }

    //没找到
    if(!found)
        goto _ConvertDevicePathToDosPathExit;

    //这里我们找到了dos名和设备名的对应关系
    length = symbolName.Length + DevicePath->Length - devName.Length + 5;
    if(Allocate)
    {
        OutBuffer->Buffer = ExAllocatePoolWithTag(PoolType,
            length,
            'nabu');
        OutBuffer->Length = 0;
        OutBuffer->MaximumLength = (USHORT)length;
    }

    if(OutBuffer->MaximumLength < length)
        goto _ConvertDevicePathToDosPathExit;
    OutBuffer->Length = 0;

    //dos磁盘路径头部
    pwchar = OutBuffer->Buffer;
    RtlCopyMemory(pwchar, (PWCHAR)symbolName.Buffer + 4, symbolName.Length - 4 * sizeof(WCHAR));
    pwchar += symbolName.Length / sizeof(WCHAR) - 4;

    //路径
    pwchar2 = (PWCHAR)DevicePath->Buffer + devName.Length / sizeof(WCHAR);
    length = DevicePath->Length - devName.Length;
    RtlCopyMemory(pwchar, pwchar2, length);
    pwchar += length / sizeof(WCHAR);

    OutBuffer->Length = (pwchar - OutBuffer->Buffer) * sizeof(WCHAR);
    ret = TRUE;

_ConvertDevicePathToDosPathExit:
    ExFreePool(devName.Buffer);
    return ret;
}


//打开或关闭内存写保护
VOID
EnableWritePretect(BOOLEAN Enable)
{
    if(Enable)
    {
        __asm {
            push eax;
            mov eax, cr0;
            or eax, 10000h;
            mov cr0, eax;
            pop eax;
            sti;
        }
    }
    else
    {
        __asm {
            cli;
            push eax;
            mov eax, cr0;
            and eax, 0FFFEFFFFh;
            mov cr0, eax;
            pop eax;
        }
    }
}

//挂钩Function函数至FakeFunction
//JmpBuffer是FakeFunction跳转回Function的跳转缓冲区
BOOLEAN
HookProcess(PVOID Function, PVOID FakeFunction, PUCHAR JmpBuffer)
{
    ULONG length;
    UCHAR jmpCode[5];
    PUCHAR temp;

    length = ade32_get_code_length(Function, 5);
    if(length == 0)
        return FALSE;

    temp = (PUCHAR)Function + length;
    RtlCopyMemory(JmpBuffer, Function, length);

    JmpBuffer[length] = 0xe9;
    *(PULONG)(JmpBuffer + length + 1) = ((PUCHAR)Function + length - (JmpBuffer + length) - 5);

    jmpCode[0] = 0xe9;
    *(PULONG)(&jmpCode[1]) = (ULONG)((PUCHAR)FakeFunction - (PUCHAR)Function - 5);

    EnableWritePretect(FALSE);
    RtlCopyMemory(Function, jmpCode, 5);
    EnableWritePretect(TRUE);

    return TRUE;
}