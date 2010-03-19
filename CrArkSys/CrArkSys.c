#include <ntddk.h>
#include <string.h>
#include "CrArkSys.h"
#include "Dispatch.h"
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
    PVOID inputBuffer, outputBuffer;
    ULONG inputLength, outputLength, *information;

    inputBuffer = irpSp->Parameters.DeviceIoControl.Type3InputBuffer;
    inputLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
    outputBuffer = Irp->UserBuffer;
    outputLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
    information = &Irp->IoStatus.Information;

	switch(irpSp->Parameters.DeviceIoControl.IoControlCode)
	{
    case IOCTL_CRARKSYS_HELLO:
        KdPrint(("Hello World!!\n"));
        Irp->IoStatus.Status = STATUS_SUCCESS;
        Irp->IoStatus.Information = 0;
        break;

    case IOCTL_CRARKSYS_SPEINIT:
        Irp->IoStatus.Status = DispatchSpecialInitialize(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

    case IOCTL_CRARKSYS_PROCENUM:
        Irp->IoStatus.Status = DispatchProcessEnum(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

    case IOCTL_CRARKSYS_THREADENUM:
        Irp->IoStatus.Status = DispatchThreadEnum(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

    case IOCTL_CRARKSYS_QUERYPROCNAME:
        Irp->IoStatus.Status = DispatchQueryProcessName(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

    case IOCTL_CRARKSYS_QUERYPROCINFO:
        Irp->IoStatus.Status = DispatchQueryProcessInfo(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

    case IOCTL_CRARKSYS_QUERYTHREADINFO:
        Irp->IoStatus.Status = DispatchQueryThreadInfo(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

    case IOCTL_CRARKSYS_QUERYPROCMODULE:
        Irp->IoStatus.Status = DispatchQueryProcessModuleList(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

    case IOCTL_CRARKSYS_QUERYMODULEINFO:
        Irp->IoStatus.Status = DispatchQueryProcessModuleInfo(inputBuffer, inputLength, outputBuffer, outputLength, information);
        break;

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

    KdPrint(("CrArkSys Unload.\n"));
}

NTSTATUS DriverEntry(
	IN OUT PDRIVER_OBJECT   DriverObject,
	IN PUNICODE_STRING      RegistryPath
	)
{
	PDEVICE_OBJECT pdoDeviceObj = 0;
	NTSTATUS status = STATUS_UNSUCCESSFUL;

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

    KdPrint(("CrArkSys DriverLoad.\n"));
	return STATUS_SUCCESS;
}
