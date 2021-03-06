using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class AppliAudioIO : MonoBehaviour
{

	private const string STR_CLASS = "AppliAudioIO";

	static private AppliAudioIO instance	= null;
	static private bool isInitialized		= false;

#if UNITY_ANDROID
	static private AndroidJavaObject androidJavaObject = null;
#elif UNITY_IPHONE
	[DllImport("__Internal")]
	static private extern void createAudioIOPlugin();
	[DllImport("__Internal")]
	static private extern void destroyAudioIOPlugin();
	[DllImport("__Internal")]
	static private extern int getSamplesPerBufferAudioIOPlugin();
	[DllImport("__Internal")]
	static private extern void setSamplesPerBufferAudioIOPlugin(int size);
	[DllImport("__Internal")]
	static private extern float getInputVolumeAudioIOPlugin();
	[DllImport("__Internal")]
	static private extern void setInputVolumeAudioIOPlugin(float value);
	[DllImport("__Internal")]
	static private extern float getOutputVolumeAudioIOPlugin();
	[DllImport("__Internal")]
	static private extern void setOutputVolumeAudioIOPlugin(float value);
	[DllImport("__Internal")]
	static private extern IntPtr getMessageAudioIOPlugin();
#endif

	static public void Init()
	{
		AppliDebug.D(STR_CLASS, "Init()");
		if (isInitialized)
		{
			return;
		}
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("createAudioIOPlugin");
#elif UNITY_IPHONE
		createAudioIOPlugin();
#endif
		isInitialized = true;
	}
	
	static public void Term()
	{
		AppliDebug.D(STR_CLASS, "Term()");
		if (!isInitialized)
		{
			return;
		}
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("destroyAudioIOPlugin");
#elif UNITY_IPHONE
		destroyAudioIOPlugin();
#endif
		isInitialized = false;
	}

	static public int GetSamplesPerBuffer()
	{
		int result = 1024;
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<int>("getSamplesPerBufferAudioIOPlugin");
#elif UNITY_IPHONE
		result = getSamplesPerBufferAudioIOPlugin();
#endif
		return result;
	}
	
	static public void SetSamplesPerBuffer(int value)
	{
		AppliDebug.D(STR_CLASS, "SetSamplesPerBuffer() value : " + value);
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("setSamplesPerBufferAudioIOPlugin", new object[]{value});
#elif UNITY_IPHONE
		setSamplesPerBufferAudioIOPlugin(value);
#endif
	}
	
	static public float GetInputVolume()
	{
		float result = 0.0f;
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<float>("getInputVolumeAudioIOPlugin");
#elif UNITY_IPHONE
		result = getInputVolumeAudioIOPlugin();
#endif
		return result;
	}
	
	static public void SetInputVolume(float value)
	{
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("setInputVolumeAudioIOPlugin", new object[]{value});
#elif UNITY_IPHONE
		setInputVolumeAudioIOPlugin(value);
#endif
	}
	
	static public float GetOutputVolume()
	{
		float result = 0.0f;
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		result = GetAndroidJavaObject().CallStatic<float>("getOutputVolumeAudioIOPlugin");
#elif UNITY_IPHONE
		result = getOutputVolumeAudioIOPlugin();
#endif
		return result;
	}

	static public void SetOutputVolume(float value)
	{
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
		//
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		//
#elif UNITY_ANDROID
		GetAndroidJavaObject().CallStatic("setOutputVolumeAudioIOPlugin", new object[]{value});
#elif UNITY_IPHONE
		setOutputVolumeAudioIOPlugin(value);
#endif
	}

	static public void Callback(string arg)
	{
		//AppliDebug.D(STR_CLASS, "Callback() " + arg);
		if (arg != null)
		{
			try
			{
				string[] args = arg.Split(',');
				if (args != null && 2 <= args.Length)
				{
					if (string.Equals(args[0], "INIT"))
					{
						instance.StartCoroutine(Main.OnStart());
					}
					else if (string.Equals(args[0], "Interruption"))
					{
						Main.OnResume();
					}
					else if (string.Equals(args[0], "LOG"))
					{
						AppliDebug.D(STR_CLASS, "Callback() log : " + args[1]);
					}
				}
			}
			catch (Exception e)
			{
				AppliDebug.W(STR_CLASS, "Callback() error : " + e);
			}
		}
	}

	void Awake()
	{
		AppliDebug.D(STR_CLASS, "Awake()");
		instance = this;
	}

	void Update()
	{
		if (isInitialized)
		{
			while (true)
			{
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
				break;
#elif UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
				break;
#elif UNITY_ANDROID
				string str = GetAndroidJavaObject().CallStatic<string>("getMessageAudioIOPlugin");
				if (string.IsNullOrEmpty(str))
				{
					break;
				}
				else
				{
					Receiver.instance.AudioIOFromOS(str);
				}
#elif UNITY_IPHONE
				IntPtr ptr = getMessageAudioIOPlugin();
				if (ptr == IntPtr.Zero)
				{
					break;
				}
				else
				{
					Receiver.instance.AudioIOFromOS(Marshal.PtrToStringAnsi(ptr));
				}
#else
				break;
#endif
			}
		}
	}

#if !UNITY_EDITOR && UNITY_ANDROID
	static private AndroidJavaObject GetAndroidJavaObject()
	{
		if (androidJavaObject == null)
		{
			if (26 <= AppliPlugin.GetVersionSdkInt())
			{
				AppliDebug.D(STR_CLASS, "GetAndroidJavaObject() jp.co.aquariumy.audioio.aaudio");
				androidJavaObject = new AndroidJavaObject("jp.co.aquariumy.audioio.aaudio.AudioIOPlugin");
			}
			else if (21 <= AppliPlugin.GetVersionSdkInt() && 0 <= AppliPlugin.GetManufacturer().IndexOf("SAMSUNG"))
			{
				AppliDebug.D(STR_CLASS, "GetAndroidJavaObject() jp.co.aquariumy.audioio.samsung");
				androidJavaObject = new AndroidJavaObject("jp.co.aquariumy.audioio.samsung.AudioIOPlugin");
			}
			else
			{
				AppliDebug.D(STR_CLASS, "GetAndroidJavaObject() jp.co.aquariumy.audioio");
				androidJavaObject = new AndroidJavaObject("jp.co.aquariumy.audioio.AudioIOPlugin");
			}
		}
		return androidJavaObject;
	}
#endif

}

