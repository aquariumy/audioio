package jp.co.aquariumy.audioio.aaudio;

import android.util.Log;

public class Debug
{

	static public void d(String tag, String msg)
	{
		if (BuildConfig.DEBUG)
		{
			Log.d(tag, msg);
		}
	}

	static public void w(String tag, String msg)
	{
		if (BuildConfig.DEBUG)
		{
			Log.w(tag, msg);
		}
	}

}
