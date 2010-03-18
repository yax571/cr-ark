#pragma once

#include "drvcommon.h"

#define DEVICE_NAME			"\\Device\\CRARKSYS"
#define SYMLINK_NAME		"\\DosDevices\\CRARKSYS"
PRESET_UNICODE_STRING(usDeviceName, DEVICE_NAME);
PRESET_UNICODE_STRING(usSymlinkName, SYMLINK_NAME);

// Values defined for "Method"
// METHOD_BUFFERED
// METHOD_IN_DIRECT
// METHOD_OUT_DIRECT
// METHOD_NEITHER
// 
// Values defined for "Access"
// FILE_ANY_ACCESS
// FILE_READ_ACCESS
// FILE_WRITE_ACCESS

#include "CtrlCode.h"
