//
//  ExtendedUnityAppController.m
//  Unity-iPhone
//
//  Created by root on 2014/05/11.
//
//

#import "ExtendedUnityAppController.h"
#import "Unity.h"

@implementation ExtendedUnityAppController

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [super application:application didFinishLaunchingWithOptions:launchOptions];
    [Unity call:"CON_LAUNCH"];
    return NO;
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    [Unity call:"CON_OPENURL"];
    return [super application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [super applicationDidBecomeActive:application];
    [Unity call:"CON_ACTIVE"];
}

// For iOS 4
// Callback order:
//   applicationWillEnterForeground()
//   applicationDidBecomeActive()
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    [super applicationWillEnterForeground:application];
    [Unity call:"CON_FOREGROUND"];
}

//- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
//{
//    [Unity call:"CON_NOTIFICATIONS"];
//    //[super application:application didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
//}

//- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
//{
//    //[super application:application didReceiveRemoteNotification:userInfo];
//}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    [Unity call:"CON_NOMEMORY"];
    [super applicationDidReceiveMemoryWarning:application];
}

// For iOS 4
// Callback order:
//   applicationDidResignActive()
//   applicationDidEnterBackground()
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    [Unity call:"CON_BACKGROUND"];
    [super applicationDidEnterBackground:application];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [Unity call:"CON_NOACTIVE"];
    [super applicationWillResignActive:application];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [Unity call:"CON_TERMINATE"];
    [super applicationWillTerminate:application];
}

@end
