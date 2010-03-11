#include <ntddk.h>
#include <string.h>
#include "CrArkSys.h"

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
	pdoGlobalDrvObj = DriverObject;

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

	return STATUS_SUCCESS;
}
