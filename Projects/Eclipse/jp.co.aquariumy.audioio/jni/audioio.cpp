
#include <stdio.h>
#include "mutex.h"
#include "audioin.h"
#include "audioout.h"
#include "audioio_define.h"
#include "audioio_dll.h"
#include "audioio.h"

#ifdef DEBUG
#   define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "AudioIO", __VA_ARGS__)
#   define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  "AudioIO", __VA_ARGS__)
#else
#   define LOGD(...)
#   define LOGW(...)
#endif

#define MESSAGE_NUM		(64)
#define MESSAGE_SIZE	(256)
#define BUFFER_NUM		(2)

static Mutex *lockObject;
static char resultMessage[MESSAGE_SIZE];
static char messages[MESSAGE_NUM][MESSAGE_SIZE];
static int messageRead;
static int messageWrite;
static AudioIn *in;
static AudioOut *out;
static int samplesPerBuffer;
static float inputVolume;
static float outputVolume;
static float offsetVolume;
static bool isConnectingHeadphone;
static short *bufferRec;
static float *bufferInL;
static float *bufferInR;
static float *bufferOutL;
static float *bufferOutR;
static int bufferIndexIn;
static int bufferIndexOut;
static SLObjectItf slEngineObjectItf;
static SLEngineItf slEngineItf;

#ifdef __cplusplus
extern "C"
{
#endif

_declspec(dllexport) void createAudioIOPlugin()
{
	messageRead		= 0;
	messageWrite	= 0;
	pushMessage("LOG,audioio createAudioIOPlugin()");
	lockObject			= NULL;
	lockObject			= new Mutex();
	in					= NULL;
	out					= NULL;
	samplesPerBuffer	    = 512;
    inputVolume             = 1.0f;
	outputVolume		    = 1.0f;
    offsetVolume            = 1.0f;
    isConnectingHeadphone   = false;
	bufferRec               = NULL;
	bufferInL			    = NULL;
	bufferInR			    = NULL;
	bufferOutL			    = NULL;
	bufferOutR		   	    = NULL;
	bufferIndexIn		    = 0;
	bufferIndexOut		    = 0;
	// OpenSL ES
	SLresult slResult;
	slEngineObjectItf	= NULL;
	slEngineItf			= NULL;
	LOGD("createAudioIOPlugin() Engine - slCreateEngine");
	SLEngineOption slEngineOption[]		= {(SLuint32)SL_ENGINEOPTION_THREADSAFE, (SLuint32)SL_BOOLEAN_TRUE};
	SLInterfaceID slEngineInterfaceID[]	= {SL_IID_ENGINE};
	SLboolean slEngineBoolean[]			= {SL_BOOLEAN_TRUE};
	slResult							= slCreateEngine(&slEngineObjectItf, _countof(slEngineOption), slEngineOption, _countof(slEngineInterfaceID), slEngineInterfaceID, slEngineBoolean);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("createAudioIOPlugin() Engine - slCreateEngine error %u", slResult);
	}
	LOGD("createAudioIOPlugin() Engine - Realize");
	slResult = (*slEngineObjectItf)->Realize(slEngineObjectItf, SL_BOOLEAN_FALSE);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("createAudioIOPlugin() Engine - Realize error %u", slResult);
	}
	LOGD("createAudioIOPlugin() Engine - GetInterface SL_IID_ENGINE");
	slResult = (*slEngineObjectItf)->GetInterface(slEngineObjectItf, SL_IID_ENGINE, &slEngineItf);
	if (slResult != SL_RESULT_SUCCESS)
	{
		LOGW("createAudioIOPlugin() Engine - GetInterface SL_IID_ENGINE error %u", slResult);
	}
	setSamplesPerBufferAudioIOPlugin(samplesPerBuffer);
}

_declspec(dllexport) void destroyAudioIOPlugin()
{
	pushMessage("LOG,audioio destroyAudioIOPlugin()");
	if (in != NULL)
	{
		delete in;
		in = NULL;
	}
	if (out != NULL)
	{
		delete out;
		out = NULL;
	}
	if (bufferRec != NULL)
	{
		free(bufferRec);
		bufferRec = NULL;
	}
	if (bufferInL != NULL)
	{
		free(bufferInL);
		bufferInL = NULL;
	}
	if (bufferInR != NULL)
	{
		free(bufferInR);
		bufferInR = NULL;
	}
	if (bufferOutL != NULL)
	{
		free(bufferOutL);
		bufferOutL = NULL;
	}
	if (bufferOutR != NULL)
	{
		free(bufferOutR);
		bufferOutR = NULL;
	}
    if (lockObject != NULL)
    {
        delete lockObject;
        lockObject = NULL;
    }
	// OpenSL ES
	if (slEngineObjectItf != NULL)
	{
		(*slEngineObjectItf)->Destroy(slEngineObjectItf);
		slEngineObjectItf = NULL;
	}
}

_declspec(dllexport) int getSamplesPerBufferAudioIOPlugin()
{
	return samplesPerBuffer;
}

_declspec(dllexport) void setSamplesPerBufferAudioIOPlugin(int value)
{
	if (in != NULL)
	{
		delete in;
		in = NULL;
	}
	if (out != NULL)
	{
		delete out;
		out = NULL;
	}
	if (bufferRec != NULL)
	{
		free(bufferRec);
		bufferRec = NULL;
	}
	if (bufferInL != NULL)
	{
		free(bufferInL);
		bufferInL = NULL;
	}
	if (bufferInR != NULL)
	{
		free(bufferInR);
		bufferInR = NULL;
	}
	if (bufferOutL != NULL)
	{
		free(bufferOutL);
		bufferOutL = NULL;
	}
	if (bufferOutR != NULL)
	{
		free(bufferOutR);
		bufferOutR = NULL;
	}
	samplesPerBuffer	= value;
	bufferRec			= (short *)malloc(samplesPerBuffer * CHANNEL_MONO * sizeof(short) * BUFFER_NUM);
	bufferInL			= (float *)malloc(samplesPerBuffer * sizeof(float));
	bufferInR			= (float *)malloc(samplesPerBuffer * sizeof(float));
	bufferOutL			= (float *)malloc(samplesPerBuffer * sizeof(float));
	bufferOutR			= (float *)malloc(samplesPerBuffer * sizeof(float));
	bufferIndexIn		= 0;
	bufferIndexOut		= 0;
	in					= new AudioIn(slEngineItf, samplesPerBuffer, audioIn);
	out					= new AudioOut(slEngineItf, samplesPerBuffer, audioOut);
}

_declspec(dllexport) float getInputVolumeAudioIOPlugin()
{
    return inputVolume;
}

_declspec(dllexport) void setInputVolumeAudioIOPlugin(float value)
{
    inputVolume = value;
}

_declspec(dllexport) float getOutputVolumeAudioIOPlugin()
{
	return outputVolume;
}

_declspec(dllexport) void setOutputVolumeAudioIOPlugin(float value)
{
	outputVolume = value;
}

_declspec(dllexport) char *getMessageAudioIOPlugin()
{
	return popMessage();
}

_declspec(dllexport) void setHeadphoneFlagAudioIOPlugin(int flag)
{
    isConnectingHeadphone = (flag == 1);
}

void pushMessage(const char *m)
{
	memset(messages[messageWrite], 0, MESSAGE_SIZE);
	strcpy(messages[messageWrite], m);
	messageWrite = (messageWrite + 1) % MESSAGE_NUM;
}

void pushMessageAndValue(const char *m, long v)
{
	memset(messages[messageWrite], 0, MESSAGE_SIZE);
	sprintf(messages[messageWrite], "%s : %ld", m, v);
	messageWrite = (messageWrite + 1) % MESSAGE_NUM;
}

void pushMessageAndFValue(const char *m, float f)
{
	memset(messages[messageWrite], 0, MESSAGE_SIZE);
	sprintf(messages[messageWrite], "%s : %f", m, f);
	messageWrite = (messageWrite + 1) % MESSAGE_NUM;
}

void pushMessageAndText(const char *m, char *t)
{
	memset(messages[messageWrite], 0, MESSAGE_SIZE);
	sprintf(messages[messageWrite], "%s : %s", m, t);
	messageWrite = (messageWrite + 1) % MESSAGE_NUM;
}

void pushMessageAndPtr(const char *m, void *p)
{
	memset(messages[messageWrite], 0, MESSAGE_SIZE);
	sprintf(messages[messageWrite], "%s : 0x%p", m, p);
	messageWrite = (messageWrite + 1) % MESSAGE_NUM;
}

char *popMessage()
{
	char *result = NULL;
	if (messageRead == messageWrite)
	{
		//
	}
	else
	{
		result = messages[messageRead];
		messageRead = (messageRead + 1) % MESSAGE_NUM;
	}
	return result;
}

void audioIn(short *buffer)
{
	//LOGD("audioIn() bufferIndexIn : %d", bufferIndexIn);
	short *p = bufferRec;
	p += (samplesPerBuffer * CHANNEL_MONO * bufferIndexIn);
	memcpy(p, buffer, samplesPerBuffer * CHANNEL_MONO * sizeof(short));
	bufferIndexIn = (bufferIndexIn + 1) % BUFFER_NUM;
}

void audioOut(short *buffer)
{
	//LOGD("audioOut() bufferIndexOut : %d", bufferIndexOut);
	short *p = bufferRec;
	p += (samplesPerBuffer * CHANNEL_MONO * bufferIndexOut);
    for (int i = 0; i < samplesPerBuffer; i++)
    {
        float value     = (float)(*p) * inputVolume / 32768.0f;
        bufferInL[i]    = value;
        bufferInR[i]    = value;
        p++;
    }
    int size = samplesPerBuffer * CHANNEL_MONO * sizeof(float);
    memcpy(bufferOutL, bufferInL, size);
    memcpy(bufferOutR, bufferInR, size);
    lockObject->lock();
    {
    	// something
    }
    lockObject->unlock();
    if (isConnectingHeadphone)
    {
        offsetVolume += 0.004f;
        if (1.0f < offsetVolume)
        {
            offsetVolume = 1.0f;
        }
    }
    else
    {
        offsetVolume = 0.0f;
    }
    float v = outputVolume * offsetVolume;
    for (int i = 0; i < samplesPerBuffer; i++)
    {
        *buffer++ = (short)(bufferOutL[i] * v * 32767.0f);
        *buffer++ = (short)(bufferOutR[i] * v * 32767.0f);
    }
	bufferIndexOut = (bufferIndexOut + 1) % BUFFER_NUM;
}

#ifdef __cplusplus
}
#endif
