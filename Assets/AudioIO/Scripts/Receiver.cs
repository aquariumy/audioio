using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class Receiver : MonoBehaviour
{

	static public Receiver instance;
	
	private const string STR_CLASS = "Receiver";

#if UNITY_ANDROID
	private const int ANDROID_KEYCODE_DPAD_UP		= 19;
	private const int ANDROID_KEYCODE_DPAD_DOWN		= 20;
	private const int ANDROID_KEYCODE_DPAD_LEFT		= 21;
	private const int ANDROID_KEYCODE_DPAD_RIGHT	= 22;
	private const int ANDROID_KEYCODE_DPAD_CENTER	= 23;
	private const int ANDROID_KEYCODE_ENTER			= 66;
	private const int ANDROID_KEYCODE_NUMPAD_ENTER	= 160;	

	public void CallFromAndroid(string arg)
	{
		AppliDebug.D(STR_CLASS, "CallFromAndroid() " + arg);
		if (arg.Equals("ACT_CREATE"))
		{
			//
		}
		else if (arg.Equals("ACT_START"))
		{
			//
		}
		else if (arg.Equals("ACT_RESUME"))
		{
			Main.OnResume();
		}
		else if (arg.Equals("ACT_PAUSE"))
		{
			//
		}
		else if (arg.Equals("ACT_STOP"))
		{
			//
		}
		else if (arg.Equals("ACT_DESTROY"))
		{
			//
		}
		else if (arg.Equals("WEBVIEW_ERROR"))
		{
			//
		}
		else if (arg.Equals("WEBVIEW_FINISH"))
		{
			//
		}
	}
#endif
	
#if UNITY_IPHONE
	public void CallFromIOS(string arg)
	{
		AppliDebug.D(STR_CLASS, "CallFromIOS() " + arg);
		if (arg.Equals("CON_LAUNCH"))
		{
			//
		}
		else if (arg.Equals("CON_OPENURL"))
		{
			//
		}
		else if (arg.Equals("CON_ACTIVE"))
		{
			//
		}
		else if (arg.Equals("CON_FOREGROUND"))
		{
			Main.OnResume();
		}
		else if (arg.Equals("CON_NOTIFICATIONS"))
		{
			//
		}
		else if (arg.Equals("CON_NOMEMORY"))
		{
			//
		}
		else if (arg.Equals("CON_BACKGROUND"))
		{
			//
		}
		else if (arg.Equals("CON_NOACTIVE"))
		{
			//
		}
		else if (arg.Equals("CON_TERMINATE"))
		{
			//
		}
	}
#endif

	public void AudioIOFromOS(string arg)
	{
		AppliAudioIO.Callback(arg);
	}

	public void LogDFromOS(string arg)
	{
		AppliDebug.D(STR_CLASS, "LogDFromOS() " + arg);
	}
	
	public void LogWFromOS(string arg)
	{
		AppliDebug.W(STR_CLASS, "LogWFromOS() " + arg);
	}

	void Awake()
	{
		AppliDebug.D(STR_CLASS, "Awake()");
		instance = this;
	}

}

