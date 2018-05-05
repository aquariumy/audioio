
#include "audioio_define.h"

class AudioIn
{

typedef void (*audioInCallback)(short *buffer);

public:
	AudioIn(SLEngineItf slEngineItf, int samplesPerBuffer, audioInCallback audioIn);
	~AudioIn();

	audioInCallback aic;
	int samples;
	short *buffer;
	int bufferIndex;

	SLObjectItf slRecorderObjectItf;
	SLRecordItf slRecordItf;
	SLAndroidSimpleBufferQueueItf slAndroidSimpleBufferQueueItf;

private:
	static void slInputBufferQueueCallbackEntry(SLAndroidSimpleBufferQueueItf caller, void *pContext);

};