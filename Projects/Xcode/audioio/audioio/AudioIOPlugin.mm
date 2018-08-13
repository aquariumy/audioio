
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <AVFoundation/AVFoundation.h>
#include "debug.h"
#include "audioio.h"
#include "audioio_define.h"
#include "audioio_func.h"
#include "AudioIOPlugin.h"

#define MESSAGE_NUM     (64)
#define MESSAGE_SIZE    (256)
#define BUFFER_NUM		(2)

@interface AudioSessionReceiver : NSObject

- (void)handleAVAudioSessionInterruptionNotification:(NSNotification *)notification;
- (void)handleAVAudioSessionRouteChangeNotification:(NSNotification *)notification;

@end

static char messages[MESSAGE_NUM][MESSAGE_SIZE];
static int messageRead;
static int messageWrite;

static NSObject *lockObject;
static AudioSessionReceiver *asr;
static AudioIO *io;
static int samplesPerBuffer;
static float inputVolume;
static float outputVolume;
static float offsetVolume;
static BOOL isConnectingHeadphone;
static short *bufferRec;
static float *bufferInL;
static float *bufferInR;
static float *bufferOutL;
static float *bufferOutR;

#ifdef __cplusplus
extern "C"
{
#endif

    BOOL checkConnectingHeadphone(NSArray *outputs)
    {
        for (AVAudioSessionPortDescription *desc in outputs)
        {
            if ([desc.portType isEqual:AVAudioSessionPortHeadphones])
            {
                return YES;
            }
        }
        return NO;
    };
    
    _declspec(dllexport) void createAudioIOPlugin()
    {
        LOG_D("createAudioIOPlugin()")
        NSError *error;
        messageRead		= 0;
        messageWrite    = 0;
        pushMessage("LOG,audioio createAudioIOPlugin()");
        AVAudioSession *as      = [AVAudioSession sharedInstance];
        lockObject              = [[NSObject alloc] init];
        io                      = nil;
        samplesPerBuffer        = 64;
        inputVolume             = 1.0f;
        outputVolume            = 1.0f;
        offsetVolume            = 1.0f;
        isConnectingHeadphone   = checkConnectingHeadphone([[as currentRoute] outputs]);
        bufferRec               = (short *)malloc(SAMPLES_MAX_PER_BUFFER * CHANNEL_MONO * sizeof(short));
        bufferInL               = (float *)malloc(SAMPLES_MAX_PER_BUFFER * sizeof(float));
        bufferInR               = (float *)malloc(SAMPLES_MAX_PER_BUFFER * sizeof(float));
        bufferOutL              = (float *)malloc(SAMPLES_MAX_PER_BUFFER * sizeof(float));
        bufferOutR              = (float *)malloc(SAMPLES_MAX_PER_BUFFER * sizeof(float));
        asr                     = [[AudioSessionReceiver alloc] init];
        [as setCategory:AVAudioSessionCategoryPlayAndRecord error:&error];
        [as setActive:YES error:&error];
        NSNotificationCenter *ns = [NSNotificationCenter defaultCenter];
        [ns addObserver:asr
               selector:@selector(handleAVAudioSessionInterruptionNotification:)
                   name:AVAudioSessionInterruptionNotification
                 object:nil];
        [ns addObserver:asr
               selector:@selector(handleAVAudioSessionRouteChangeNotification:)
                   name:AVAudioSessionRouteChangeNotification
                 object:nil];
        setSamplesPerBufferAudioIOPlugin(samplesPerBuffer);
        io = [[AudioIO alloc] initWithAudioIOCallback:audioIO];
        LOG_D_VALUE("isConnectingHeadphone", isConnectingHeadphone);
    }
    
    _declspec(dllexport) void destroyAudioIOPlugin()
    {
        pushMessage("LOG,audioio destroyAudioIOPlugin()");
        if (io != nil)
        {
            [io release];
            io = nil;
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
        if (asr != nil)
        {
            NSNotificationCenter *ns = [NSNotificationCenter defaultCenter];
            [ns removeObserver:asr];
            [asr release];
            asr = nil;
        }
        if (lockObject != nil)
        {
            [lockObject release];
            lockObject = nil;
        }
    }
    
    _declspec(dllexport) int getSamplesPerBufferAudioIOPlugin()
    {
        return samplesPerBuffer;
    }
    
    _declspec(dllexport) void setSamplesPerBufferAudioIOPlugin(int value)
    {
        NSError *error;
        samplesPerBuffer            = value;
        AVAudioSession *as          = [AVAudioSession sharedInstance];
        NSTimeInterval timeInterval = (NSTimeInterval)samplesPerBuffer / (NSTimeInterval)SAMPLES_PER_SEC;
        //NSLog(@"setSamplesPerBufferAudioIOPlugin() timeInterval : %f", timeInterval);
        [as setPreferredIOBufferDuration:timeInterval error:&error];
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
    
    void audioIO(int samples, short *buffer)
    {
        memcpy(bufferRec, buffer, samples * CHANNEL_MONO * sizeof(short));
        short *p = bufferRec;
        for (int i = 0; i < samples; i++)
        {
            float value     = (float)(*p) * inputVolume / 32768.0f;
            bufferInL[i]    = value;
            bufferInR[i]    = value;
            p++;
        }
        int size = samples * CHANNEL_MONO * sizeof(float);
        memcpy(bufferOutL, bufferInL, size);
        memcpy(bufferOutR, bufferInR, size);
        @synchronized(lockObject)
        {
            // something
        }
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
        for (int i = 0; i < samples; i++)
        {
            *buffer++                       = (short)(bufferOutL[i] * v * 32767.0f);
            *buffer++                       = (short)(bufferOutR[i] * v * 32767.0f);
        }
    }
    
#ifdef __cplusplus
}
#endif

@implementation AudioSessionReceiver

- (void)handleAVAudioSessionInterruptionNotification:(NSNotification *)notification
{
    NSNumber *type = [notification.userInfo objectForKey:AVAudioSessionInterruptionTypeKey];
    if (type)
    {
        if (type.unsignedIntegerValue == AVAudioSessionInterruptionTypeBegan)
        {
            LOG_D("AVAudioSessionInterruptionTypeBegan");
        }
        else if (type.unsignedIntegerValue == AVAudioSessionInterruptionTypeEnded)
        {
            LOG_D("AVAudioSessionInterruptionTypeEnded");
            pushMessage("Interruption,");
        }
    }
}

- (void)handleAVAudioSessionRouteChangeNotification:(NSNotification *)notification
{
    NSNumber *reason = [notification.userInfo objectForKey:AVAudioSessionRouteChangeReasonKey];
    if (reason)
    {
        switch (reason.unsignedIntegerValue)
        {
            case AVAudioSessionRouteChangeReasonUnknown:
                LOG_D("AVAudioSessionRouteChangeReasonUnknown");
                break;
            case AVAudioSessionRouteChangeReasonNewDeviceAvailable:
                LOG_D("AVAudioSessionRouteChangeReasonNewDeviceAvailable");
                break;
            case AVAudioSessionRouteChangeReasonOldDeviceUnavailable:
                LOG_D("AVAudioSessionRouteChangeReasonOldDeviceUnavailable");
                break;
            case AVAudioSessionRouteChangeReasonCategoryChange:
                LOG_D("AVAudioSessionRouteChangeReasonCategoryChange");
                break;
            case AVAudioSessionRouteChangeReasonOverride:
                LOG_D("AVAudioSessionRouteChangeReasonOverride");
                break;
            case AVAudioSessionRouteChangeReasonWakeFromSleep:
                LOG_D("AVAudioSessionRouteChangeReasonWakeFromSleep");
                break;
            case AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory:
                LOG_D("AVAudioSessionRouteChangeReasonNoSuitableRouteForCategory");
                break;
            case AVAudioSessionRouteChangeReasonRouteConfigurationChange:
                LOG_D("AVAudioSessionRouteChangeReasonRouteConfigurationChange");
                break;
        }
    }
    AVAudioSessionRouteDescription *prevDesc = notification.userInfo[AVAudioSessionRouteChangePreviousRouteKey];
    if (checkConnectingHeadphone([[[AVAudioSession sharedInstance] currentRoute] outputs]))
    {
        if (!checkConnectingHeadphone(prevDesc.outputs))
        {
            isConnectingHeadphone = YES;
        }
    }
    else
    {
        if (checkConnectingHeadphone(prevDesc.outputs))
        {
            isConnectingHeadphone = NO;
        }
    }
}

@end

