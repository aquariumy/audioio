
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "math.h"
#include "audioin.h"

#ifdef DEBUG
#   define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "AudioIn", __VA_ARGS__)
#   define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  "AudioIn", __VA_ARGS__)
#else
#   define LOGD(...)
#   define LOGW(...)
#endif

#define BUFFER_NUM (2)

AudioIn::AudioIn(SLEngineItf slEngineItf, int samplesPerBuffer, audioInCallback audioIn)
{
	LOGD("AudioIn()");
	SLresult slResult;
	samples							= samplesPerBuffer;
	aic								= audioIn;
	buffer							= NULL;
	buffer							= (short *)calloc(samples * CHANNEL_MONO * BUFFER_NUM, sizeof(short));
	slRecorderObjectItf				= NULL;
	slRecordItf						= NULL;
	slAndroidSimpleBufferQueueItf	= NULL;
	LOGD("AudioIn() CreateAudioRecorder");
	SLDataLocator_IODevice slDataLocatorIODevice;
	SLDataSource slDataSource;
	SLDataLocator_AndroidSimpleBufferQueue slDataLocatorAndroidSimpleBufferQueue;
    SLDataFormat_PCM slDataFormatPCM;
	SLDataSink slDataSink;
	memset(&slDataLocatorIODevice,					0, sizeof(slDataLocatorIODevice));
	memset(&slDataSource,							0, sizeof(slDataSource));
	memset(&slDataLocatorAndroidSimpleBufferQueue,	0, sizeof(slDataLocatorAndroidSimpleBufferQueue));
	memset(&slDataFormatPCM,						0, sizeof(slDataFormatPCM));
	memset(&slDataSink,								0, sizeof(slDataSink));
	slDataLocatorIODevice.locatorType					= SL_DATALOCATOR_IODEVICE;
	slDataLocatorIODevice.deviceType					= SL_IODEVICE_AUDIOINPUT;
	slDataLocatorIODevice.deviceID						= SL_DEFAULTDEVICEID_AUDIOINPUT;
	slDataLocatorIODevice.device						= NULL;
	slDataSource.pLocator								= &slDataLocatorIODevice;
	slDataSource.pFormat								= NULL;
	slDataLocatorAndroidSimpleBufferQueue.locatorType	= SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
	slDataLocatorAndroidSimpleBufferQueue.numBuffers	= BUFFER_NUM;
    slDataFormatPCM.formatType							= SL_DATAFORMAT_PCM;
    slDataFormatPCM.numChannels							= CHANNEL_MONO;
    slDataFormatPCM.samplesPerSec						= SL_SAMPLINGRATE_48;
    slDataFormatPCM.bitsPerSample						= SL_PCMSAMPLEFORMAT_FIXED_16;
    slDataFormatPCM.containerSize						= slDataFormatPCM.bitsPerSample;
    slDataFormatPCM.channelMask							= SL_SPEAKER_FRONT_CENTER;
    slDataFormatPCM.endianness							= SL_BYTEORDER_LITTLEENDIAN;
	slDataSink.pLocator									= &slDataLocatorAndroidSimpleBufferQueue;
	slDataSink.pFormat									= &slDataFormatPCM;
	SLInterfaceID slRecorderInterfaceID[]				= {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
	SLboolean slRecorderBoolean[]						= {SL_BOOLEAN_TRUE};
	slResult											= (*slEngineItf)->CreateAudioRecorder(slEngineItf, &slRecorderObjectItf, &slDataSource, &slDataSink, _countof(slRecorderInterfaceID), slRecorderInterfaceID, slRecorderBoolean);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioIn() CreateAudioRecorder error %u", slResult);
		return;
	}
	LOGD("AudioIn() Realize");
	slResult = (*slRecorderObjectItf)->Realize(slRecorderObjectItf, SL_BOOLEAN_FALSE);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioIn() Realize error %u", slResult);
		return;
	}
	LOGD("AudioIn() GetInterface SL_IID_RECORD");	
	slResult = (*slRecorderObjectItf)->GetInterface(slRecorderObjectItf, SL_IID_RECORD, &slRecordItf);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioIn() GetInterface SL_IID_RECORD error %u", slResult);
		return;
	}
	LOGD("AudioIn() GetInterface SL_IID_ANDROIDSIMPLEBUFFERQUEUE");
	slResult = (*slRecorderObjectItf)->GetInterface(slRecorderObjectItf, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &slAndroidSimpleBufferQueueItf);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioIn() GetInterface SL_IID_ANDROIDSIMPLEBUFFERQUEUE error %u", slResult);
		return;
	}
	LOGD("AudioIn() RegisterCallback slOutputBufferQueueCallbackEntry");
	slResult = (*slAndroidSimpleBufferQueueItf)->RegisterCallback(slAndroidSimpleBufferQueueItf, slInputBufferQueueCallbackEntry, this);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioIn() RegisterCallback error %u", slResult);
		return;
	}
	for (int i = 0; i < BUFFER_NUM; i++)
	{
		LOGD("AudioIn() Enqueue");
		short *p = buffer;
		p += (samples * CHANNEL_MONO * i);
		slResult = (*slAndroidSimpleBufferQueueItf)->Enqueue(slAndroidSimpleBufferQueueItf, p, samples * CHANNEL_MONO * sizeof(short));
		if (slResult != SL_RESULT_SUCCESS)
		{
			LOGW("AudioIn() Enqueue error %u", slResult);
			return;
		}
	}
	bufferIndex = 0;
	LOGD("AudioIn() SetRecordState");
	slResult = (*slRecordItf)->SetRecordState(slRecordItf, SL_RECORDSTATE_RECORDING);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioIn() SetRecordState error %u", slResult);
		return;
	}
}

AudioIn::~AudioIn()
{
	LOGD("~AudioIn()");
	SLresult slResult;
    if (slRecordItf != NULL)
    {
        LOGD("~AudioIn() SetRecordState");
        slResult = (*slRecordItf)->SetRecordState(slRecordItf, SL_RECORDSTATE_STOPPED);
        if (slResult != SL_RESULT_SUCCESS)
        {
            LOGW("~AudioIn() SetRecordState error %u", slResult);
        }
    }	
    if (slRecorderObjectItf != NULL)
    {
        (*slRecorderObjectItf)->Destroy(slRecorderObjectItf);
        slRecorderObjectItf = NULL;
    }
	if (buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}    
}

void AudioIn::slInputBufferQueueCallbackEntry(SLAndroidSimpleBufferQueueItf caller, void *pContext)
{
	SLresult slResult;
	AudioIn *cls	= (AudioIn *)pContext;
	short *p		= cls->buffer;
	p += (cls->samples * CHANNEL_MONO * cls->bufferIndex);
	cls->aic(p);
	slResult = (*caller)->Enqueue(caller, p, cls->samples * CHANNEL_MONO * sizeof(short));
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("slInputBufferQueueCallbackEntry() Enqueue error %u", slResult);
	}
	cls->bufferIndex = (cls->bufferIndex + 1) % BUFFER_NUM;
}

