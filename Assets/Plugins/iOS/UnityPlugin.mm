//
//  UnityPlugin.c
//  Unity-iPhone
//
//  Created by root on 2014/05/11.
//
//

#import "Unity.h"
#import "UnityPlugin.h"

extern UIWindow         *UnityGetMainWindow();
extern UIViewController *UnityGetGLViewController();
extern UIView           *UnityGetGLView();

static NSString                 *version                            = nil;
static UIView                   *inputGuardView                     = nil;
static UIActivityIndicatorView  *activityIndicatorView              = nil;
static NSString                 *setVisibleLoadingViewPlugin_option = nil;

void createPlugin()
{
    version = [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
    [version retain];
    if (!inputGuardView)
    {
        inputGuardView                  = [[UIView alloc] initWithFrame:UnityGetGLView().frame];
        inputGuardView.backgroundColor  = [UIColor blackColor];
        inputGuardView.alpha            = 0.5f;
        inputGuardView.hidden           = YES;
        [UnityGetMainWindow() addSubview:inputGuardView];
    }
    if (!activityIndicatorView)
    {
        activityIndicatorView                               = [[UIActivityIndicatorView alloc] initWithFrame:UnityGetGLView().frame];
        activityIndicatorView.activityIndicatorViewStyle    = UIActivityIndicatorViewStyleWhiteLarge;
        activityIndicatorView.hidesWhenStopped              = YES;
        [activityIndicatorView stopAnimating];
        [UnityGetMainWindow() addSubview:activityIndicatorView];
    }
}

void destroyPlugin()
{
    if (version)
    {
        [version release];
        version = nil;
    }
    if (inputGuardView)
    {
        [inputGuardView removeFromSuperview];
        [inputGuardView release];
        inputGuardView = nil;
    }
    if (activityIndicatorView)
    {
        [activityIndicatorView removeFromSuperview];
        [activityIndicatorView release];
        activityIndicatorView = nil;
    }
}

char *getVersionPlugin()
{
    char *result = (char *)calloc([version length] + 1, sizeof(char));
    strcpy(result, [version UTF8String]);
    return result;
}

int getScreenWidthPlugin()
{
    CGRect screen	= [[UIScreen mainScreen] bounds];
    int w			= (int)CGRectGetWidth(screen);
    int h			= (int)CGRectGetHeight(screen);
	return w < h ? w : h;
}

int getScreenHeightPlugin()
{
    CGRect screen	= [[UIScreen mainScreen] bounds];
    int w			= (int)CGRectGetWidth(screen);
    int h			= (int)CGRectGetHeight(screen);
	return w < h ? h : w;
}

void setVisibleLoadingViewPlugin(char *option)
{
    if (setVisibleLoadingViewPlugin_option)
    {
        return;
    }
    setVisibleLoadingViewPlugin_option = [NSString stringWithCString:option encoding:NSUTF8StringEncoding];
    [setVisibleLoadingViewPlugin_option retain];
	dispatch_async(dispatch_get_main_queue(),
                   ^{
                       inputGuardView.hidden = [setVisibleLoadingViewPlugin_option isEqualToString:@"transparent"];
                       [activityIndicatorView startAnimating];
                       [setVisibleLoadingViewPlugin_option release];
                       setVisibleLoadingViewPlugin_option = nil;
                   });
}

void setInvisibleLoadingViewPlugin()
{
	dispatch_async(dispatch_get_main_queue(),
                   ^{
                       inputGuardView.hidden = YES;
                       [activityIndicatorView stopAnimating];
                   });
}

void logPlugin(char *message)
{
	NSLog(@"%s", message);
}


