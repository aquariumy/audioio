using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public class Main : MonoBehaviour
{

	public enum State
	{
		Init	= 0,
		Main	= 1
	}

	static public State state = State.Init;

	private const string STR_CLASS = "Main";

	private const int SAMPLES_PER_BUFFER_DEFAULT	= 3;
	private const int SAMPLES_PER_BUFFER_NUM		= 6;

	private const float PARAM_DB_MIN = -40.0f;
	private const float PARAM_DB_MAX =   0.0f;

	static private Main instance = null;

	[SerializeField]
	private Toggle[] samplesPerBufferToggles = new Toggle[SAMPLES_PER_BUFFER_NUM];
	[SerializeField]
	private Slider inputVolumeSlider;
	[SerializeField]
	private Slider outputVolumeSlider;
	[SerializeField]
	private Text inputVolumeText;
	[SerializeField]
	private Text outputVolumeText;

	private int samplesPerBufferIndex	= SAMPLES_PER_BUFFER_DEFAULT;
	private float inputVolume			= 1.0f;
	private float outputVolume			= 1.0f;

	static public IEnumerator OnStart()
	{
		AppliDebug.D(STR_CLASS, "OnStart()");
        yield return new WaitForSeconds(1.0f);
		// AppliAudioIO
		AppliAudioIO.Init();
		// UI
		AppliPlugin.SetInvisibleLoadingView();
		// Debug
		AppliDebug.D(STR_CLASS, "OnStart() version-sdk-int : " + AppliPlugin.GetVersionSdkInt());
		AppliDebug.D(STR_CLASS, "OnStart() manufacturer : " + AppliPlugin.GetManufacturer());
		AppliDebug.D(STR_CLASS, "OnStart() model : " + AppliPlugin.GetModel());
		state = State.Main;
	}

	static public void OnResume()
	{
		AppliDebug.D(STR_CLASS, "OnResume()");
#if !UNITY_EDITOR && UNITY_IPHONE
		AppliAudioIO.Term();
		AppliAudioIO.Init();
#endif
		if (instance == null || state == State.Init)
		{
			// 初期化中
		}
		else
		{
#if !UNITY_EDITOR && UNITY_IPHONE
			instance.SetSamplesPerBuffer(instance.samplesPerBufferIndex);
			instance.inputVolumeSlider.value	= instance.inputVolume;
			instance.outputVolumeSlider.value	= instance.outputVolume;
#endif
		}
	}

	static public void OnClickBack()
	{
		AppliDebug.D(STR_CLASS, "OnClickBack()");
#if !UNITY_EDITOR && UNITY_ANDROID
		Application.Quit();
#endif
	}

	public void OnClickSamplesPerBuffer(int index)
	{
		if (instance == null || state == State.Init)
		{
			// 初期化中
		}
		else
		{
			SetSamplesPerBuffer(index);
			samplesPerBufferIndex = index;
		}
	}

	public void OnChangeSliderInput()
	{
		if (instance == null || state == State.Init)
		{
			// 初期化中
		}
		else
		{
			float dbVolume		= CalcParamDB(inputVolumeSlider.value); 
			float linearVolume	= GetLinearVolume(dbVolume);
			AppliAudioIO.SetInputVolume(linearVolume);
			inputVolumeText.text	= dbVolume.ToString("F1") + " dB";
			inputVolume				= inputVolumeSlider.value;
		}
	}
	
	public void OnChangeSliderOutput()
	{
		if (instance == null || state == State.Init)
		{
			// 初期化中
		}
		else
		{
			float dbVolume		= CalcParamDB(outputVolumeSlider.value); 
			float linearVolume	= GetLinearVolume(dbVolume);
			AppliAudioIO.SetOutputVolume(linearVolume);
			outputVolumeText.text	= dbVolume.ToString("F1") + " dB";
			outputVolume			= outputVolumeSlider.value;
		}
	}

	public void OnClickReset()
	{
		AppliDebug.D(STR_CLASS, "OnClickReset()");
		AppliPlugin.SetVisibleLoadingView("");
		SetSamplesPerBuffer(SAMPLES_PER_BUFFER_DEFAULT);
		samplesPerBufferToggles[SAMPLES_PER_BUFFER_DEFAULT].isOn	= true;
		inputVolumeSlider.value										= 1.0f;
		outputVolumeSlider.value									= 1.0f;
		AppliPlugin.SetInvisibleLoadingView();
	}

	void Start()
	{
		AppliDebug.D(STR_CLASS, "Start()");
		instance = this;
		StartCoroutine(Init());
	}

	void Update()
	{
#if !UNITY_EDITOR && UNITY_ANDROID
		if (Input.GetKeyDown(KeyCode.Escape))
		{
			Main.OnClickBack();
		}
#endif
	}

	void OnDestroy()
	{
		AppliDebug.D(STR_CLASS, "OnDestroy()");
		Term();
	}
	
	// アプリケーションの開始処理
	private IEnumerator Init()
	{
		AppliDebug.D(STR_CLASS, "Init()");
		// AppliPlugin
		yield return StartCoroutine(AppliPlugin.Init());
		AppliPlugin.SetVisibleLoadingView("");
		// Version
		AppliDebug.D(STR_CLASS, "Init() version : " + AppliPlugin.GetVersion());
#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN || UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		StartCoroutine(OnStart());
#elif UNITY_ANDROID
		AppliPlugin.RequestUsbPermission();
#elif UNITY_IPHONE
		yield return new WaitForSeconds(1.0f);
		StartCoroutine(OnStart());
#endif
		AppliDebug.D(STR_CLASS, "Init() finish");
	}

	// アプリケーションの終了処理
	private void Term()
	{
		AppliDebug.D(STR_CLASS, "Term()");
		// AppliPlugin
		AppliPlugin.Term();
		// AppliAudioIO
		AppliAudioIO.Term();
	}

	private float CalcParamDB(float value)
	{
		return PARAM_DB_MIN + ((PARAM_DB_MAX - PARAM_DB_MIN) * value);
	}
	
	private float GetDBVolume(float linearVolume)
	{
		return 20.0f * Mathf.Log10(linearVolume);
	}
	
	private float GetLinearVolume(float dBVolume)
	{
		return Mathf.Pow(10.0f, dBVolume / 20.0f);
	}

	private void SetSamplesPerBuffer(int index)
	{
		switch (index)
		{
			case 0 :
				AppliAudioIO.SetSamplesPerBuffer(64);
				break;
			case 1 :
				AppliAudioIO.SetSamplesPerBuffer(128);
				break;
			case 2 :
				AppliAudioIO.SetSamplesPerBuffer(256);
				break;
			case 3 :
				AppliAudioIO.SetSamplesPerBuffer(512);
				break;
			case 4 :
				AppliAudioIO.SetSamplesPerBuffer(768);
				break;
			case 5 :
				AppliAudioIO.SetSamplesPerBuffer(1024);
				break;
		}
	}

}

