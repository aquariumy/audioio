
#include <stdio.h>
#include "debug.h"
#include "audioio_define.h"
#include "audioio.h"

static OSStatus callbackEntry(void *inRefCon,
                              AudioUnitRenderActionFlags *ioActionFlags,
                              const AudioTimeStamp *inTimeStamp,
                              UInt32 inBusNumber,
                              UInt32 inNumberFrames,
                              AudioBufferList *ioData)
{
    memset(ioData->mBuffers[0].mData, 0, ioData->mBuffers[0].mDataByteSize);
    if (inBusNumber == 0)
    {
        /*
        LOG_D_UVALUE("inNumberFrames ", (unsigned int)inNumberFrames);
        LOG_D_UVALUE("mNumberBuffers ", (unsigned int)ioData->mNumberBuffers);
        for (int i = 0; i < ioData->mNumberBuffers; i++)
        {
            LOG_D_UVALUE("mNumberChannels", (unsigned int)ioData->mBuffers[i].mNumberChannels);
            LOG_D_UVALUE("mDataByteSize  ", (unsigned int)ioData->mBuffers[i].mDataByteSize);
        }
        //*/
        OSStatus status;
        AudioIO *cls = (AudioIO *)inRefCon;
        status = AudioUnitRender(cls->unitIO, ioActionFlags, inTimeStamp, 1, inNumberFrames, ioData);
        if (status == 0)
        {
            cls->aioc((int)inNumberFrames, (short *)ioData->mBuffers[0].mData);
        }
        else
        {
            LOG_W_VALUE("error", (int)status);
        }
    }
    return 0;
}

@implementation AudioIO

- (id)initWithAudioIOCallback:(audioIOCallback)audioIO
{
    LOG_D("");
    self = [super init];
    if (self)
    {
        OSStatus status;
        aioc                    = audioIO;
        AudioUnitScope ausOut   = 0;
        AudioUnitScope ausIn    = 1;
        //UInt32 disable          = 0;
        UInt32 enable           = 1;
        // AudioStreamBasicDescription
        AudioStreamBasicDescription asbd;
        memset(&asbd, 0, sizeof(asbd));
        asbd.mFormatID          = kAudioFormatLinearPCM;
        asbd.mFormatFlags       = kLinearPCMFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
        asbd.mSampleRate        = SAMPLES_PER_SEC;
        asbd.mBitsPerChannel    = BITS_PER_SAMPLE;
        asbd.mChannelsPerFrame  = CHANNEL_MONO;
        asbd.mBytesPerFrame     = (asbd.mBitsPerChannel / 8) * asbd.mChannelsPerFrame;
        asbd.mFramesPerPacket   = 1;
        asbd.mBytesPerPacket    = asbd.mBytesPerFrame * asbd.mFramesPerPacket;
        // AURenderCallbackStruct
        AURenderCallbackStruct aurcs    = {0,};
        aurcs.inputProc                 = callbackEntry;
        aurcs.inputProcRefCon           = (void *)self;
        // AUGraph
        AudioComponentDescription acd = {0,};
        status = NewAUGraph(&graph);
        if (status != 0)
        {
            LOG_W_VALUE("NewAUGraph error", (int)status);
        }
        acd.componentType           = kAudioUnitType_Output;
        acd.componentSubType        = kAudioUnitSubType_RemoteIO;
        acd.componentManufacturer   = kAudioUnitManufacturer_Apple;
        status = AUGraphAddNode(graph, &acd, &nodeIO);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphAddNode nodeIO error", (int)status);
        }
        acd.componentType           = kAudioUnitType_Mixer;
        acd.componentSubType        = kAudioUnitSubType_MultiChannelMixer;
        acd.componentManufacturer   = kAudioUnitManufacturer_Apple;
        status = AUGraphAddNode(graph, &acd, &nodeMixer);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphAddNode nodeMixer error", (int)status);
        }
        status = AUGraphOpen(graph);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphOpen error", (int)status);
        }
        status = AUGraphNodeInfo(graph, nodeIO, NULL, &unitIO);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphOpen nodeIO error", (int)status);
        }
        status = AUGraphNodeInfo(graph, nodeMixer, NULL, &unitMixer);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphOpen nodeMixer error", (int)status);
        }
        // AudioUnit
        status = AudioUnitSetProperty(unitIO, kAudioOutputUnitProperty_EnableIO, kAudioUnitScope_Input, ausIn, &enable, sizeof(enable));
        if (status != 0)
        {
            LOG_W_VALUE("AudioUnitSetProperty kAudioOutputUnitProperty_EnableIO error", (int)status);
        }
        status = AudioUnitSetProperty(unitIO, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, ausOut, &asbd, sizeof(asbd));
        if (status != 0)
        {
            LOG_W_VALUE("AudioUnitSetProperty kAudioUnitProperty_StreamFormat kAudioUnitScope_Input error", (int)status);
        }
        status = AudioUnitSetProperty(unitIO, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Output, ausIn, &asbd, sizeof(asbd));
        if (status != 0)
        {
            LOG_W_VALUE("AudioUnitSetProperty kAudioUnitProperty_StreamFormat kAudioUnitScope_Output error", (int)status);
        }
        UInt32 mixerInputs = 2;
        status = AudioUnitSetProperty(unitMixer, kAudioUnitProperty_ElementCount, kAudioUnitScope_Input, 0, &mixerInputs, sizeof(mixerInputs));
        if (status != 0)
        {
            LOG_W_VALUE("AudioUnitSetProperty kAudioUnitProperty_ElementCount error", (int)status);
        }
        // AUGraph
        for (UInt32 i = 0; i < mixerInputs; i++)
        {
            status = AUGraphSetNodeInputCallback(graph, nodeMixer, i, &aurcs);
            if (status != 0)
            {
                LOG_W_VALUE("AUGraphSetNodeInputCallback error", (int)status);
            }
            AudioStreamBasicDescription asbd;
            memset(&asbd, 0, sizeof(asbd));
            asbd.mFormatID          = kAudioFormatLinearPCM;
            asbd.mFormatFlags       = kLinearPCMFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
            asbd.mSampleRate        = SAMPLES_PER_SEC;
            asbd.mBitsPerChannel    = BITS_PER_SAMPLE;
            asbd.mChannelsPerFrame  = CHANNEL_STEREO;
            asbd.mBytesPerFrame     = (asbd.mBitsPerChannel / 8) * asbd.mChannelsPerFrame;
            asbd.mFramesPerPacket   = 1;
            asbd.mBytesPerPacket    = asbd.mBytesPerFrame * asbd.mFramesPerPacket;
            status = AudioUnitSetProperty(unitMixer, kAudioUnitProperty_StreamFormat, kAudioUnitScope_Input, i, &asbd, sizeof(asbd));
            if (status != 0)
            {
                LOG_W_VALUE("AudioUnitSetProperty kAudioUnitProperty_StreamFormat error", (int)status);
            }
        }
        status = AUGraphConnectNodeInput(graph, nodeMixer, 0, nodeIO, 0);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphConnectNodeInput error", (int)status);
        }
        status = AUGraphInitialize(graph);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphInitialize error", (int)status);
        }
        status = AUGraphStart(graph);
        if (status != 0)
        {
            LOG_W_VALUE("AUGraphStart error", (int)status);
        }
    }
    LOG_D("finish");
    return self;
}

- (void)dealloc
{
    if (graph != NULL)
    {
        OSStatus status;
        Boolean b;
        AUGraphStop(graph);
        status = AUGraphIsInitialized(graph, &b);
        if (status == 0 && b)
        {
            AUGraphUninitialize(graph);
        }
        status = AUGraphIsOpen(graph, &b);
        if (status == 0 && b)
        {
            AUGraphClose(graph);
        }
        DisposeAUGraph(graph);
        graph = NULL;
    }
    [super dealloc];
}

@end

