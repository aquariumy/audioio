
#include "audioio_define.h"

class AudioOut
{

typedef void (*audioOutCallback)(short *buffer);

public:
	AudioOut(SLEngineItf slEngineItf, int samplesPerBuffer, audioOutCallback audioOut);
	~AudioOut();

	audioOutCallback aoc;
	int samples;
	short *buffer;
	int bufferIndex;

	SLObjectItf slOutputMixObjectItf;
	SLObjectItf slPlayerObjectItf;
	SLPlayItf slPlayItf;
	SLAndroidSimpleBufferQueueItf slAndroidSimpleBufferQueueItf;

private:
	static void slOutputBufferQueueCallbackEntry(SLAndroidSimpleBufferQueueItf caller, void *pContext);

};