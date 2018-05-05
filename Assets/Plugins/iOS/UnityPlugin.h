//
//  UnityPlugin.h
//  Unity-iPhone
//
//  Created by root on 2014/05/11.
//
//

#ifdef __cplusplus
extern "C"
{
#endif
	void createPlugin();
	void destroyPlugin();
	char *getVersionPlugin();
	int getScreenWidthPlugin();
	int getScreenHeightPlugin();
	void setVisibleLoadingViewPlugin(char *option);
	void setInvisibleLoadingViewPlugin();
	void logPlugin(char *message);
#ifdef __cplusplus
}
#endif


