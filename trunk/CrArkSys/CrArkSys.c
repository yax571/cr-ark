#include <ntddk.h>
#include <string.h>
#include "CrArkSys.h"
#include "HashTable.h"
#include "Enviroment.h"
#include "ProcEnum.h"
#include "Query.h"
#include "helper.h"

PDRIVER_OBJECT pdoGlobalDrvObj = 0;

NTSTATUS CRARKSYS_DispatchCreateClose(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS CRARKSYS_DispatchDeviceControl(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);

	switch(irpSp->Parameters.DeviceIoControl.IoControlCode)
	{
	default:
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		break;
	}

	status = Irp->IoStatus.Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

VOID CRARKSYS_DriverUnload(
	IN PDRIVER_OBJECT		DriverObject
	)
{
	PDEVICE_OBJECT pdoNextDeviceObj = pdoGlobalDrvObj->DeviceObject;
    //为安全考虑
    UnhookFunction(KeInsertQueueApc, KeInsertQueueApcJumpBack);

	IoDeleteSymbolicLink(&usSymlinkName);

	// Delete all the device objects
	while(pdoNextDeviceObj)
	{
		PDEVICE_OBJECT pdoThisDeviceObj = pdoNextDeviceObj;
		pdoNextDeviceObj = pdoThisDeviceObj->NextDevice;
		IoDeleteDevice(pdoThisDeviceObj);
	}
}

NTSTATUS DriverEntry(
	IN OUT PDRIVER_OBJECT   DriverObject,
	IN PUNICODE_STRING      RegistryPath
	)
{
	PDEVICE_OBJECT pdoDeviceObj = 0;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
    ULONG i, j;
    PObjectIdTable objIdTbl, threadobjIdTbl;
    PEPROCESS process;
    PProcessNameInfo processNameInfo;
    PProcessModuleList kernelModList, r3modList;
    PThreadInfo threadInfo;
    PProcessModuleList moduleList;
    PModuleInfo moduleInfo;

	pdoGlobalDrvObj = DriverObject;

    //首先初始化各种变量
    if(EnviromentInitialize(DriverObject) == FALSE)
        return status;

    
	// Create the device object.
	if(!NT_SUCCESS(status = IoCreateDevice(
		DriverObject,
		0,
		&usDeviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&pdoDeviceObj
		)))
	{
		// Bail out (implicitly forces the driver to unload).
		return status;
	};

	// Now create the respective symbolic link object
	if(!NT_SUCCESS(status = IoCreateSymbolicLink(
		&usSymlinkName,
		&usDeviceName
		)))
	{
		IoDeleteDevice(pdoDeviceObj);
		return status;
	}

	// NOTE: You need not provide your own implementation for any major function that
	//       you do not want to handle. I have seen code using DDKWizard that left the
	//       *empty* dispatch routines intact. This is not necessary at all!
	DriverObject->MajorFunction[IRP_MJ_CREATE] =
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CRARKSYS_DispatchCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = CRARKSYS_DispatchDeviceControl;
	DriverObject->DriverUnload = CRARKSYS_DriverUnload;

    objIdTbl = ProcessEnum(TRUE);
    if(objIdTbl == NULL)
    {
        KdPrint(("enum fail.\n"));
    }
    else
    {
        KdPrint(("process total number: %lu\n", objIdTbl->Count));
        for(i = 0; i < objIdTbl->Count; i++)
        {
            KdPrint(("EPROCESS: %8.8X\tPID: %lu\n", objIdTbl->Entry[i].Object, objIdTbl->Entry[i].UniqueId));
            processNameInfo = QueryProcessName(objIdTbl->Entry[i].Object);
            if(processNameInfo != NULL)
            {
                KdPrint(("\t ImageName: %s\tFullPath: %s\n", processNameInfo->ImageName, processNameInfo->FullPath));
                ExFreePool(processNameInfo);
            }
            threadobjIdTbl = ThreadEnum(objIdTbl->Entry[i].Object);
            if(threadobjIdTbl)
            {
                for(j = 0; j < threadobjIdTbl->Count; j++)
                {
                    threadInfo = NULL;
                    threadInfo = QueryThreadInfo((PETHREAD)threadobjIdTbl->Entry[j].Object);
                    if(threadInfo)
                    {
                        KdPrint(("\tETHREAD: %8.8X\tTid:%lu\tswitchs:%lu\tpriority:%lu\tstartaddress:%8.8X\tstate:%lu\tpath: %s\n",
                                 threadInfo->EThread, threadInfo->Tid, threadInfo->ContextSwitches, threadInfo->BasePriority, threadInfo->StartAddress, threadInfo->State, threadInfo->ImagePath));
                        ExFreePool(threadInfo);
                    }
                }
                if(i == 5)
                    EnviromentSpecialInitialize(NULL, threadobjIdTbl->Entry[2].Object, FALSE);
                FreeObjIdTable(threadobjIdTbl);
            }
//             moduleList = QueryProcessModuleList(objIdTbl->Entry[i].Object);
//             if(moduleList)
//             {
//                 for(j = 0; j < moduleList->Count; j++)
//                 {
//                     moduleInfo = QueryModuleInfo(moduleList->Process, moduleList->LdrDataTable[j]);
//                     if(moduleInfo)
//                     {
//                         KdPrint(("ImageBase: %8.8X\tEntryPoint: %8.8X\tSizeOfImage:%8.8X\tFullPath: %s\n",
//                                 moduleInfo->BaseAddress,
//                                 moduleInfo->EntryPoint,
//                                 moduleInfo->SizeOfImage,
//                                 moduleInfo->FullPath));
//                         ExFreePool(moduleInfo);
//                     }
//                 }
//                 ExFreePool(moduleList);
//             }
        }
        FreeObjIdTable(objIdTbl);
    }
	return STATUS_SUCCESS;
}
