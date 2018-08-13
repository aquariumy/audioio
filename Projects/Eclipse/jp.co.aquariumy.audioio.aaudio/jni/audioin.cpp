
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

AudioIn::AudioIn(int samplesPerBuffer, audioInCallback audioIn)
{
	LOGD("AudioIn()");
	aaudio_result_t result;
	samples	= samplesPerBuffer;
	aic		= audioIn;
	builder	= NULL;
	stream 	= NULL;
	LOGD("AudioIn() AAudio_createStreamBuilder");
  	result = AAudio_createStreamBuilder(&builder);
	if (result != AAUDIO_OK)
	{
		LOGW("AudioIn() AAudio_createStreamBuilder error %s", AAudio_convertResultToText(result));
		return;
	}
  	//AAudioStreamBuilder_setDeviceId(builder, DEVICE_ID);
  	AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
  	AAudioStreamBuilder_setChannelCount(builder, CHANNEL_MONO);
  	AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);
  	AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
  	AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_INPUT);
  	AAudioStreamBuilder_setSampleRate(builder, SAMPLES_PER_SEC);
  	AAudioStreamBuilder_setDataCallback(builder, dataCallback, this);
  	AAudioStreamBuilder_setErrorCallback(builder, errorCallback, this);
	LOGD("AudioIn() AAudioStreamBuilder_openStream");
    result = AAudioStreamBuilder_openStream(builder, &stream);
	if (result != AAUDIO_OK)
	{
		LOGW("AudioIn() AAudioStreamBuilder_openStream error %s", AAudio_convertResultToText(result));
		return;
	}
	if (stream == NULL)
	{
		LOGW("AudioIn() stream is NULL");
		return;
	}
	LOGD("AudioIn() AAudioStream_getDeviceId : %d", AAudioStream_getDeviceId(stream));
	LOGD("AudioIn() AAudioStream_getBufferCapacityInFrames : %d", AAudioStream_getBufferCapacityInFrames(stream));
	LOGD("AudioIn() AAudioStream_getBufferSizeInFrames : %d", AAudioStream_getBufferSizeInFrames(stream));
	LOGD("AudioIn() AAudioStream_requestStart");
    result = AAudioStream_requestStart(stream);
	if (result != AAUDIO_OK)
	{
		LOGW("AudioIn() AAudioStream_requestStart error %s", AAudio_convertResultToText(result));
		return;
	}
}

AudioIn::~AudioIn()
{
	LOGD("~AudioIn()");
	aaudio_result_t result;
	if (stream != NULL)
	{
		LOGD("~AudioIn() AAudioStream_requestStop");
		result = AAudioStream_requestStop(stream);
    	if (result != AAUDIO_OK)
    	{
			LOGW("AudioIn() AAudioStream_requestStop error %s", AAudio_convertResultToText(result));
		}
		LOGD("~AudioIn() AAudioStream_close");
		result = AAudioStream_close(stream);
		if (result != AAUDIO_OK)
		{
			LOGW("AudioIn() AAudioStream_close error %s", AAudio_convertResultToText(result));
    	}
    	stream = NULL;
	}
	if (builder != NULL)
	{
		LOGD("~AudioIn() AAudioStreamBuilder_delete");
		result = AAudioStreamBuilder_delete(builder);
    	if (result != AAUDIO_OK)
    	{
			LOGW("AudioIn() AAudioStreamBuilder_delete error %s", AAudio_convertResultToText(result));
		}
    	builder = NULL;
	}	
}

aaudio_data_callback_result_t AudioIn::dataCallback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames)
{
	AudioIn *cls = (AudioIn *)userData;
	cls->aic((short *)audioData, numFrames);
	return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void AudioIn::errorCallback(AAudioStream *stream, void *userData, int error)
{
	LOGW("errorCallback() error %s", AAudio_convertResultToText(error));
	aaudio_stream_state_t streamState = AAudioStream_getState(stream);
	LOGW("errorCallback() state %s", AAudio_convertStreamStateToText(streamState));
	if (streamState == AAUDIO_STREAM_STATE_DISCONNECTED)
	{
		//
	}
}