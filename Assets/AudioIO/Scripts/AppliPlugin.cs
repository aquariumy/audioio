using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class AppliPlugin
{

	private const string STR_CLASS = "AppliPlugin";

	static private bool initialized = false;

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
	//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
	//
#elif UNITY_ANDROID
	static private AndroidJavaObject androidJavaObject = null;
#elif UNITY_IPHONE
	[DllImport("__Internal")]
	static private extern void createPlugin();
	[DllImport("__Internal")]
	static private extern void destroyPlugin();
	[DllImport("__Internal")]
	static private extern string getVersionPlugin();
	[DllImport("__Internal")]
	static private extern int getScreenWidthPlugin();
	[DllImport("__Internal")]
	static private extern int getScreenHeightPlugin();
	[DllImport("__Internal")]
	static private extern void setVisibleLoadingViewPlugin(string option);
	[DllImport("__Internal")]
	static private extern void setInvisibleLoadingViewPlugin();
	[DllImport("__Internal")]
	static private extern void logPlugin(string message);
#endif

	static public IEnumerator Init()
	{
		AppliDebug.D(STR_CLASS, "Init()");
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("createPlugin");
#elif UNITY_IPHONE
		createPlugin();
#endif
		initialized = true;
		yield break;
	}

	static public void Term()
	{
		AppliDebug.D(STR_CLASS, "Term()");
		if (!initialized)
		{
			return;
		}
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("destroyPlugin");
#elif UNITY_IPHONE
		destroyPlugin();
#endif
	}

	static public string GetVersion()
	{
		string result = "";
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		result = "Editor";
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		result = "Editor";
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<string>("getVersionPlugin");
#elif UNITY_IPHONE
		result = getVersionPlugin();
#endif
		return result;
	}

	static public int GetScreenWidth()
	{
		int result = Screen.width;
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		result = Screen.width;
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		result = Screen.width;
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<int>("getScreenWidthPlugin");
#elif UNITY_IPHONE
		result = getScreenWidthPlugin();
#endif
		return result;
	}
	
	static public int GetScreenHeight()
	{
		int result = Screen.height;
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		result = Screen.height;
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		result = Screen.height;
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<int>("getScreenHeightPlugin");
#elif UNITY_IPHONE
		result = getScreenHeightPlugin();
#endif
		return result;
	}
	
	static public void SetVisibleLoadingView(string option)
	{
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("setVisibleLoadingViewPlugin", new object[]{option});
#elif UNITY_IPHONE
		setVisibleLoadingViewPlugin(option);
#endif
	}
	
	static public void SetInvisibleLoadingView()
	{
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("setInvisibleLoadingViewPlugin");
#elif UNITY_IPHONE
		setInvisibleLoadingViewPlugin();
#endif
	}

	static public int GetVersionSdkInt()
	{
		int result = 0;
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<int>("getVersionSdkIntPlugin");
#elif UNITY_IPHONE
		//
#endif
		return result;
	}

	static public string GetManufacturer()
	{
		string result = "";
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<string>("getManufacturerPlugin");
#elif UNITY_IPHONE
		//
#endif
		return result;
	}
	
	static public string GetModel()
	{
		string result = "";
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<string>("getModelPlugin");
#elif UNITY_IPHONE
		//
#endif
		return result;
	}

	static public void RequestUsbPermission()
	{
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("requestUsbPermission");
#elif UNITY_IPHONE
		//
#endif
	}

	static public void Log(string message)
	{
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		//
#elif UNITY_IPHONE
		logPlugin(message);
#endif
	}

#if !UNITY_EDITOR && UNITY_ANDROID
	static private AndroidJavaObject GetAndroidJavaObject()
	{
		if (androidJavaObject == null)
		{
			androidJavaObject = new AndroidJavaObject("jp.co.aquariumy.audioio.UnityPlugin");
		}
		return androidJavaObject;
	}
#endif

}

