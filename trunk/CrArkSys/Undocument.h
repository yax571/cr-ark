#pragma once

typedef struct _LDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY      InLoadOrderLinks;
    LIST_ENTRY        InMemoryOrderLinks;
    LIST_ENTRY        InInitializationOrderLinks;
    PVOID                DllBase;
    PVOID                EntryPoint;
    ULONG              SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG               Flags;
    USHORT             LoadCount;
    USHORT             TlsIndex;
    union
    {
        LIST_ENTRY    HashLinks;
        struct
        {
            PVOID        SectionPointer;
            ULONG      CheckSum;
        };
    };
    union
    {
        ULONG            TimeDateStamp;
        PVOID           LoadedImports;
    };
    PVOID               EntryPointActivationContext;
    PVOID                PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

typedef BOOLEAN (*EX_ENUMERATE_HANDLE_ROUTINE)(
    IN PVOID HandleTableEntry,
    IN HANDLE Handle,
    IN PVOID EnumParameter
    );

NTKERNELAPI
BOOLEAN
ExEnumHandleTable (
                   IN PVOID HandleTable,
                   IN EX_ENUMERATE_HANDLE_ROUTINE EnumHandleProcedure,
                   IN PVOID EnumParameter,
                   OUT PHANDLE Handle OPTIONAL
                   );


#define SystemModuleInformation (11)

NTKERNELAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
                         IN ULONG SystemInformationClass,
                         IN OUT PVOID SystemInformation,
                         IN ULONG SystemInformationLength,
                         OUT PULONG ReturnLength OPTIONAL
                         );

typedef struct _SYSTEM_MODULE_INFORMATION { // Information Class 11
    ULONG Reserved[2];
    PVOID Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT Unknown;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;