
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "math.h"
#include "audioout.h"

#ifdef DEBUG
#   define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "AudioOut", __VA_ARGS__)
#   define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  "AudioOut", __VA_ARGS__)
#else
#   define LOGD(...)
#   define LOGW(...)
#endif

#define BUFFER_NUM (2)

AudioOut::AudioOut(SLEngineItf slEngineItf, int samplesPerBuffer, audioOutCallback audioOut)
{
	LOGD("AudioOut()");
	SLresult slResult;
	samples							= samplesPerBuffer;
	aoc								= audioOut;
	buffer							= NULL;
	buffer							= (short *)calloc(samples * CHANNEL_STEREO * BUFFER_NUM, sizeof(short));
	slOutputMixObjectItf			= NULL;
	slPlayerObjectItf				= NULL;
	slPlayItf						= NULL;
	slAndroidSimpleBufferQueueItf	= NULL;
	LOGD("AudioOut() OutputMix - CreateOutputMix");
	slResult = (*slEngineItf)->CreateOutputMix(slEngineItf, &slOutputMixObjectItf, 0, NULL, NULL);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() OutputMix - CreateOutputMix error %u", slResult);
		return;
	}
	LOGD("AudioOut() OutputMix - Realize");
	slResult = (*slOutputMixObjectItf)->Realize(slOutputMixObjectItf, SL_BOOLEAN_FALSE);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() OutputMix - Realize error %u", slResult);
		return;
	}
	LOGD("AudioOut() CreateAudioPlayer");
	SLDataLocator_AndroidSimpleBufferQueue slDataLocatorAndroidSimpleBufferQueue;
    SLDataFormat_PCM slDataFormatPCM;
	SLDataSource slDataSource;
	SLDataLocator_OutputMix slDataLocatorOutputMix;
	SLDataSink slDataSink;
	memset(&slDataLocatorAndroidSimpleBufferQueue,	0, sizeof(slDataLocatorAndroidSimpleBufferQueue));
	memset(&slDataFormatPCM,						0, sizeof(slDataFormatPCM));
	memset(&slDataSource,							0, sizeof(slDataSource));
	memset(&slDataLocatorOutputMix,					0, sizeof(slDataLocatorOutputMix));
	memset(&slDataSink,								0, sizeof(slDataSink));
	slDataLocatorAndroidSimpleBufferQueue.locatorType	= SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
	slDataLocatorAndroidSimpleBufferQueue.numBuffers	= BUFFER_NUM;
    slDataFormatPCM.formatType							= SL_DATAFORMAT_PCM;
    slDataFormatPCM.numChannels							= CHANNEL_STEREO;
    slDataFormatPCM.samplesPerSec						= SL_SAMPLINGRATE_48;
    slDataFormatPCM.bitsPerSample						= SL_PCMSAMPLEFORMAT_FIXED_16;
    slDataFormatPCM.containerSize						= slDataFormatPCM.bitsPerSample;
    slDataFormatPCM.channelMask							= SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
    slDataFormatPCM.endianness							= SL_BYTEORDER_LITTLEENDIAN;
	slDataSource.pLocator								= &slDataLocatorAndroidSimpleBufferQueue;
	slDataSource.pFormat								= &slDataFormatPCM;
	slDataLocatorOutputMix.locatorType					= SL_DATALOCATOR_OUTPUTMIX;
	slDataLocatorOutputMix.outputMix					= slOutputMixObjectItf;
	slDataSink.pLocator									= &slDataLocatorOutputMix;
	slDataSink.pFormat									= NULL;
	SLInterfaceID slPlayerInterfaceID[]					= {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
	SLboolean slPlayerBoolean[]							= {SL_BOOLEAN_TRUE};
	slResult											= (*slEngineItf)->CreateAudioPlayer(slEngineItf, &slPlayerObjectItf, &slDataSource, &slDataSink, _countof(slPlayerInterfaceID), slPlayerInterfaceID, slPlayerBoolean);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() CreateAudioPlayer error %u", slResult);
		return;
	}
	LOGD("AudioOut() Realize");
	slResult = (*slPlayerObjectItf)->Realize(slPlayerObjectItf, SL_BOOLEAN_FALSE);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() Realize error %u", slResult);
		return;
	}
	LOGD("AudioOut() GetInterface SL_IID_PLAY");
	slResult = (*slPlayerObjectItf)->GetInterface(slPlayerObjectItf, SL_IID_PLAY, &slPlayItf);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() GetInterface SL_IID_PLAY error %u", slResult);
		return;
	}
	LOGD("AudioOut() GetInterface SL_IID_ANDROIDSIMPLEBUFFERQUEUE");
	slResult = (*slPlayerObjectItf)->GetInterface(slPlayerObjectItf, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &slAndroidSimpleBufferQueueItf);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() GetInterface SL_IID_ANDROIDSIMPLEBUFFERQUEUE error %u", slResult);
		return;
	}
	LOGD("AudioOut() RegisterCallback slOutputBufferQueueCallbackEntry");
	slResult = (*slAndroidSimpleBufferQueueItf)->RegisterCallback(slAndroidSimpleBufferQueueItf, slOutputBufferQueueCallbackEntry, this);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() RegisterCallback error %u", slResult);
		return;
	}
	for (int i = 0; i < BUFFER_NUM; i++)
	{
		LOGD("AudioOut() Enqueue");
		short *p = buffer;
		p += (samples * CHANNEL_STEREO * i);
		slResult = (*slAndroidSimpleBufferQueueItf)->Enqueue(slAndroidSimpleBufferQueueItf, p, samples * CHANNEL_STEREO * sizeof(short));
		if (slResult != SL_RESULT_SUCCESS)
		{
			LOGW("AudioOut() Enqueue error %u", slResult);
			return;
		}
	}
	bufferIndex = 0;
	LOGD("AudioOut() SetPlayState");
	slResult = (*slPlayItf)->SetPlayState(slPlayItf, SL_PLAYSTATE_PLAYING);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("AudioOut() SetPlayState error %u", slResult);
		return;
	}
}

AudioOut::~AudioOut()
{
	LOGD("~AudioOut()");
	SLresult slResult;
	if (slPlayItf != NULL)
	{
		LOGD("~AudioOut() SetPlayState");
		slResult = (*slPlayItf)->SetPlayState(slPlayItf, SL_PLAYSTATE_STOPPED);
		if (slResult != SL_RESULT_SUCCESS)
		{
			LOGW("~AudioOut() SetPlayState error %u", slResult);
		}
	}
	if (slPlayerObjectItf != NULL)
	{
		(*slPlayerObjectItf)->Destroy(slPlayerObjectItf);
		slPlayerObjectItf = NULL;
	}
	if (slOutputMixObjectItf != NULL)
	{
		(*slOutputMixObjectItf)->Destroy(slOutputMixObjectItf);
		slOutputMixObjectItf = NULL;
	}
	if (buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}	
}

void AudioOut::slOutputBufferQueueCallbackEntry(SLAndroidSimpleBufferQueueItf caller, void *pContext)
{
	SLresult slResult;
	AudioOut *cls	= (AudioOut *)pContext;
	short *p		= cls->buffer;
	p += (cls->samples * CHANNEL_STEREO * cls->bufferIndex);
	cls->aoc(p);
	slResult = (*caller)->Enqueue(caller, p, cls->samples * CHANNEL_STEREO * sizeof(short));
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("slOutputBufferQueueCallbackEntry() Enqueue error %u", slResult);
	}
	cls->bufferIndex = (cls->bufferIndex + 1) % BUFFER_NUM;
}

