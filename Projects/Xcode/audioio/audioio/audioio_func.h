
#ifndef audioio_audioio_h
#define audioio_audioio_h

#ifdef __cplusplus
extern "C"
{
#endif
	void pushMessage(const char *m);
	void pushMessageAndValue(const char *m, long v);
	void pushMessageAndFValue(const char *m, float v);
	void pushMessageAndText(const char *m, char *t);
	void pushMessageAndPtr(const char *m, void *p);
	char *popMessage();
    void audioIO(int samples, short *buffer);
	void debug();
#ifdef __cplusplus
}
#endif

#endif
