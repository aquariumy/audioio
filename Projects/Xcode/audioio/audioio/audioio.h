
#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>

typedef void (*audioIOCallback)(int samples, short *buffer);

@interface AudioIO : NSObject
{

@public
    audioIOCallback aioc;
    AUGraph graph;
    AUNode nodeIO;
    AUNode nodeMixer;
    AudioUnit unitIO;
    AudioUnit unitMixer;
    
}

- (id)initWithAudioIOCallback:(audioIOCallback)audioIO;

@end
