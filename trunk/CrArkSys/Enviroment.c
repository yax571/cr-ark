#include <ntddk.h>
#include "Enviroment.h"
#include "ade32.h"

//EPROCESS�ṹ�е�ƫ��
ULONG EProcessUniqueProcessIdOffset;
ULONG EProcessActiveProcessLinksOffset;
ULONG EProcessObjectTableOffset;
ULONG EProcessSectionObjectOffset;
ULONG EProcessImageFileNameOffset;
ULONG EProcessThreadListHeadOffset;

//��̬��õĵ�ַ
PVOID PspCidTable;


BOOLEAN
GetPspCidTableAddress()
{
    ULONG length, functionAddress, current;
    UNICODE_STRING functionName;

    //���PsLookupProcessThreadByCid·��
    RtlInitUnicodeString(&functionName, 
                         L"PsLookupProcessThreadByCid");
    functionAddress = (ULONG)MmGetSystemRoutineAddress(&functionName);
    if(functionAddress == 0)
        return FALSE;

    //���� push    _PspCidTableָ��, ���PspCidTable��ַ
    current = functionAddress;
    length = 0;
    do{
        current += length;
        length = ade32_disasm(current);
        if(length == 6 && (*(USHORT*)current == 0x35ff || *(USHORT*)current == 0x3d8b))
        {
            current += 2;
            PspCidTable = *(PVOID*)(*(ULONG*)current);
            return TRUE;
        }
    }while(length != 0 && current <= functionAddress + 0x100);

    return FALSE; 
}


//��ʼ��һЩƫ����
BOOLEAN
EnviromentInitialize()
{
    ULONG buildNo;

    if(!PsGetVersion(NULL, NULL, &buildNo, NULL))
        return FALSE;

    switch(buildNo)
    {
    case 2600:      //winxp
        EProcessUniqueProcessIdOffset = 0x084;
        EProcessActiveProcessLinksOffset = 0x088;
        EProcessObjectTableOffset = 0x0c4;
        EProcessSectionObjectOffset = 0x138;
        EProcessImageFileNameOffset = 0x174;
        EProcessThreadListHeadOffset = 0x190;
        break;
    }

    if(GetPspCidTableAddress() == FALSE)
        return FALSE;

    return TRUE;
}