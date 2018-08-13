
#ifndef audioio_audioio_dll_h
#define audioio_audioio_dll_h

#define _declspec(p)

#ifdef __cplusplus
extern "C"
{
#endif
    _declspec(dllexport) void createAudioIOPlugin();
    _declspec(dllexport) void destroyAudioIOPlugin();
    _declspec(dllexport) int getSamplesPerBufferAudioIOPlugin();
    _declspec(dllexport) void setSamplesPerBufferAudioIOPlugin(int value);
    _declspec(dllexport) float getInputVolumeAudioIOPlugin();
    _declspec(dllexport) void setInputVolumeAudioIOPlugin(float value);
    _declspec(dllexport) float getOutputVolumeAudioIOPlugin();
    _declspec(dllexport) void setOutputVolumeAudioIOPlugin(float value);
    _declspec(dllexport) char *getMessageAudioIOPlugin();
    _declspec(dllexport) void setHeadphoneFlagAudioIOPlugin(int flag);
#ifdef __cplusplus
}
#endif

#endif
