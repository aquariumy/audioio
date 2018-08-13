
#include "audioio_define.h"

class AudioIn
{

typedef void (*audioInCallback)(short *buffer, int num);

public:
	AudioIn(int samplesPerBuffer, audioInCallback audioIn);
	~AudioIn();

	audioInCallback aic;
	int samples;

	AAudioStreamBuilder *builder;
	AAudioStream *stream;

private:
	static aaudio_data_callback_result_t dataCallback(AAudioStream *stream, void *userData, void *audioData, int32_t numFrames);
	static void errorCallback(AAudioStream *stream, void *userData, aaudio_result_t error);

};