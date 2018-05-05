
#ifndef audioio_define_h
#define audioio_define_h

#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#include <SLES/OpenSLES_Platform.h>

#ifndef _countof
#   define _countof(x) (sizeof(x) / sizeof(x[0]))
#endif

#define SAMPLES_PER_SEC		(48000)
#define BITS_PER_SAMPLE		(16)
#define CHANNEL_L			(0)
#define CHANNEL_R			(1)
#define CHANNEL_MONO		(1)
#define CHANNEL_STEREO		(2)

/*
#define SL_RESULT_SUCCESS                   ((SLuint32) 0x00000000)
#define SL_RESULT_PRECONDITIONS_VIOLATED    ((SLuint32) 0x00000001)
#define SL_RESULT_PARAMETER_INVALID         ((SLuint32) 0x00000002)
#define SL_RESULT_MEMORY_FAILURE            ((SLuint32) 0x00000003)
#define SL_RESULT_RESOURCE_ERROR            ((SLuint32) 0x00000004)
#define SL_RESULT_RESOURCE_LOST             ((SLuint32) 0x00000005)
#define SL_RESULT_IO_ERROR                  ((SLuint32) 0x00000006)
#define SL_RESULT_BUFFER_INSUFFICIENT       ((SLuint32) 0x00000007)
#define SL_RESULT_CONTENT_CORRUPTED         ((SLuint32) 0x00000008)
#define SL_RESULT_CONTENT_UNSUPPORTED       ((SLuint32) 0x00000009)
#define SL_RESULT_CONTENT_NOT_FOUND         ((SLuint32) 0x0000000A)
#define SL_RESULT_PERMISSION_DENIED         ((SLuint32) 0x0000000B)
#define SL_RESULT_FEATURE_UNSUPPORTED       ((SLuint32) 0x0000000C)
#define SL_RESULT_INTERNAL_ERROR            ((SLuint32) 0x0000000D)
#define SL_RESULT_UNKNOWN_ERROR             ((SLuint32) 0x0000000E)
#define SL_RESULT_OPERATION_ABORTED         ((SLuint32) 0x0000000F)
#define SL_RESULT_CONTROL_LOST              ((SLuint32) 0x00000010)
//*/

#endif
