#pragma once

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