
#ifndef audioio_define_h
#define audioio_define_h

#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#include <SLES/OpenSLES_Platform.h>
#include <aaudio/AAudio.h>

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
    AAUDIO_OK,
    AAUDIO_ERROR_BASE = -900, // TODO review
    AAUDIO_ERROR_DISCONNECTED,
    AAUDIO_ERROR_ILLEGAL_ARGUMENT,
    // reserved
    AAUDIO_ERROR_INTERNAL = AAUDIO_ERROR_ILLEGAL_ARGUMENT + 2,
    AAUDIO_ERROR_INVALID_STATE,
    // reserved
    // reserved
    AAUDIO_ERROR_INVALID_HANDLE = AAUDIO_ERROR_INVALID_STATE + 3,
    // reserved
    AAUDIO_ERROR_UNIMPLEMENTED = AAUDIO_ERROR_INVALID_HANDLE + 2,
    AAUDIO_ERROR_UNAVAILABLE,
    AAUDIO_ERROR_NO_FREE_HANDLES,
    AAUDIO_ERROR_NO_MEMORY,
    AAUDIO_ERROR_NULL,
    AAUDIO_ERROR_TIMEOUT,
    AAUDIO_ERROR_WOULD_BLOCK,
    AAUDIO_ERROR_INVALID_FORMAT,
    AAUDIO_ERROR_OUT_OF_RANGE,
    AAUDIO_ERROR_NO_SERVICE,
    AAUDIO_ERROR_INVALID_RATE
//*/

#endif
