
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

AudioOut::AudioOut(int samplesPerBuffer, audioOutCallback audioOut)
{
	LOGD("AudioOut()");
	aaudio_result_t result;
	samples	= samplesPerBuffer;
	aoc		= audioOut;
	builder	= NULL;
	stream 	= NULL;
	LOGD("AudioOut() AAudio_createStreamBuilder");
  	result = AAudio_createStreamBuilder(&builder);
	if (result != AAUDIO_OK)
	{
		LOGW("AudioOut() AAudio_createStreamBuilder error %s", AAudio_convertResultToText(result));
		return;
	}
  	//AAudioStreamBuilder_setDeviceId(builder, DEVICE_ID);
  	AAudioStreamBuilder_setFormat(builder, AAUDIO_FORMAT_PCM_I16);
  	AAudioStreamBuilder_setChannelCount(builder, CHANNEL_STEREO);
  	AAudioStreamBuilder_setSharingMode(builder, AAUDIO_SHARING_MODE_SHARED);
  	AAudioStreamBuilder_setPerformanceMode(builder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY);
  	AAudioStreamBuilder_setDirection(builder, AAUDIO_DIRECTION_OUTPUT);
  	AAudioStreamBuilder_setSampleRate(builder, SAMPLES_PER_SEC);
  	AAudioStreamBuilder_setDataCallback(builder, dataCallback, this);
  	AAudioStreamBuilder_setErrorCallback(builder, errorCallback, this);
	LOGD("AudioOut() AAudioStreamBuilder_openStream");
    result = AAudioStreamBuilder_openStream(builder, &stream);
	if (result != AAUDIO_OK)
	{
		LOGW("AudioOut() AAudioStreamBuilder_openStream error %s", AAudio_convertResultToText(result));
		return;
	}
	if (stream == NULL)
	{
		LOGW("AudioOut() stream is NULL");
		return;
	}
	LOGD("AudioOut() AAudioStream_getDeviceId : %d", AAudioStream_getDeviceId(stream));
	LOGD("AudioOut() AAudioStream_getBufferCapacityInFrames : %d", AAudioStream_getBufferCapacityInFrames(stream));
	LOGD("AudioOut() AAudioStream_getBufferSizeInFrames : %d", AAudioStream_getBufferSizeInFrames(stream));
	LOGD("AudioOut() AAudioStream_requestStart");
    result = AAudioStream_requestStart(stream);
	if (result != AAUDIO_OK)
	{
		LOGW("AudioOut() AAudioStream_requestStart error %s", AAudio_convertResultToText(result));
		return;
	}
}

AudioOut::~AudioOut()
{
	LOGD("~AudioOut()");
	aaudio_result_t result;
	if (stream != NULL)
	{
		LOGD("~AudioOut() AAudioStream_requestStop");
		result = AAudioStream_requestStop(stream);
    	if (result != AAUDIO_OK)
    	{
			LOGW("AudioOut() AAudioStream_requestStop error %s", AAudio_convertResultToText(result));
		}
		LOGD("~AudioOut() AAudioStream_close");
		result = AAudioStream_close(stream);
		if (result != AAUDIO_OK)
		{
			LOGW("AudioOut() AAudioStream_close error %s", AAudio_convertResultToText(result));
    	}
    	stream = NULL;
	}
	if (builder != NULL)
	{
		LOGD("~AudioOut() AAudioStreamBuilder_delete");
		result = AAudioStreamBuilder_delete(builder);
    	if (result != AAUDIO_OK)
    	{
			LOGW("AudioOut() AAudioStreamBuilder_delete error %s", AAudio_convertResultToText(result));
		}
    	builder = NULL;
	}
}

aaudio_data_callback_result_t AudioOut::dataCallback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames)
{
	AudioOut *cls = (AudioOut *)userData;
	cls->aoc((short *)audioData, numFrames);
	return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void AudioOut::errorCallback(AAudioStream *stream, void *userData, int error)
{
	LOGW("errorCallback() error %s", AAudio_convertResultToText(error));
	aaudio_stream_state_t streamState = AAudioStream_getState(stream);
	LOGW("errorCallback() state %s", AAudio_convertStreamStateToText(streamState));
	if (streamState == AAUDIO_STREAM_STATE_DISCONNECTED)
	{
		//
	}
}

