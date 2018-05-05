package jp.co.aquariumy.audioio;

import com.unity3d.player.UnityPlayer;

import android.content.Intent;
import android.content.IntentFilter;

public class AudioIOPlugin
{

	static private final String TAG = AudioIOPlugin.class.getSimpleName();

	static private HeadsetReceiver headsetReceiver = null;

	static
    {
        System.loadLibrary("audioio");
    }

	// from native
	static public void onCallback(String arg)
	{
    	Debug.d(TAG, "onCallback() arg : " + arg);
	}

	static public void createAudioIOPlugin()
	{
		create();
		// headphone
		headsetReceiver				= new HeadsetReceiver();
		IntentFilter intentFilter	= new IntentFilter();
        intentFilter.addAction(Intent.ACTION_HEADSET_PLUG);
        UnityPlayer.currentActivity.registerReceiver(headsetReceiver, intentFilter);
	}

	static public void destroyAudioIOPlugin()
	{
		destroy();
	}

	static public int getSamplesPerBufferAudioIOPlugin()
	{
		return getSamplesPerBuffer();
	}

	static public void setSamplesPerBufferAudioIOPlugin(int value)
	{
		setSamplesPerBuffer(value);
	}

	static public float getInputVolumeAudioIOPlugin()
	{
		return getInputVolume();
	}

	static public void setInputVolumeAudioIOPlugin(float value)
	{
		setInputVolume(value);
	}

	static public float getOutputVolumeAudioIOPlugin()
	{
		return getOutputVolume();
	}

	static public void setOutputVolumeAudioIOPlugin(float value)
	{
		setOutputVolume(value);
	}

	static public String getMessageAudioIOPlugin()
	{
		return getMessage();
	}

	static public void setHeadphoneFlagAudioIOPlugin(boolean flag)
	{
    	setHeadphoneFlag(flag);
	}

	static private native void create();

	static private native void destroy();

	static private native int getSamplesPerBuffer();

	static private native void setSamplesPerBuffer(int value);

	static private native float getInputVolume();

	static private native void setInputVolume(float value);

	static private native float getOutputVolume();

	static private native void setOutputVolume(float value);

	static private native String getMessage();

	static private native void setHeadphoneFlag(boolean flag);

}


