//
//  Unity.m
//  Unity-iPhone
//
//  Created by root on 2014/05/11.
//
//

#import "Unity.h"

@interface UnityLocal : NSObject

+ (void)sendMessage:(const char *)type message:(const char *)message;

@end

@implementation Unity

+ (void)call:(const char *)message
{
    [UnityLocal sendMessage:"CallFromIOS" message:message];
}

+ (void)audioio:(const char *)message
{
    [UnityLocal sendMessage:"AudioIOFromOS" message:message];
}

+ (void)d:(const char *)message
{
    [UnityLocal sendMessage:"LogDFromOS" message:message];
}

+ (void)w:(const char *)message
{
    [UnityLocal sendMessage:"LogWFromOS" message:message];
}

@end

@implementation UnityLocal

+ (void)sendMessage:(const char *)type message:(const char *)message
{
	@synchronized (self)
	{
	    UnitySendMessage("Receiver", type, message);
	}
}

@end
