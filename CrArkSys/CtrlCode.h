#pragma once

#ifndef FILE_DEVICE_CRARKSYS
#define FILE_DEVICE_CRARKSYS 0x800
#endif

#define IOCTL_CRARKSYS_HELLO CTL_CODE(FILE_DEVICE_CRARKSYS, 0x01, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_SPEINIT CTL_CODE(FILE_DEVICE_CRARKSYS, 0x02, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_PROCENUM CTL_CODE(FILE_DEVICE_CRARKSYS, 0x03, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_THREADENUM CTL_CODE(FILE_DEVICE_CRARKSYS, 0x04, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_QUERYPROCNAME CTL_CODE(FILE_DEVICE_CRARKSYS, 0x05, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_QUERYPROCINFO CTL_CODE(FILE_DEVICE_CRARKSYS, 0x06, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_QUERYTHREADINFO CTL_CODE(FILE_DEVICE_CRARKSYS, 0x07, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_QUERYPROCMODULE CTL_CODE(FILE_DEVICE_CRARKSYS, 0x08, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_QUERYMODULEINFO CTL_CODE(FILE_DEVICE_CRARKSYS, 0x09, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_TERMINATETHREAD CTL_CODE(FILE_DEVICE_CRARKSYS, 0x0a, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_TERMINATEPROC CTL_CODE(FILE_DEVICE_CRARKSYS, 0x0b, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_UNMAPMODULE CTL_CODE(FILE_DEVICE_CRARKSYS, 0x0c, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_PROTECT CTL_CODE(FILE_DEVICE_CRARKSYS, 0x0d, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_READMEM CTL_CODE(FILE_DEVICE_CRARKSYS, 0x0e, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)
#define IOCTL_CRARKSYS_WRITEMEM CTL_CODE(FILE_DEVICE_CRARKSYS, 0x0f, METHOD_NEITHER, FILE_READ_DATA | FILE_WRITE_DATA)