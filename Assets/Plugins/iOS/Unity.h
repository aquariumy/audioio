//
//  Unity.h
//  Unity-iPhone
//
//  Created by root on 2014/05/11.
//
//

@interface Unity : NSObject

+ (void)     call:(const char *)message;
+ (void)  audioio:(const char *)message;
+ (void)        d:(const char *)message;
+ (void)        w:(const char *)message;

@end
