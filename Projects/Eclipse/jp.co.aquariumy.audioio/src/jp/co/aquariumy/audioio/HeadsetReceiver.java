package jp.co.aquariumy.audioio;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class HeadsetReceiver extends BroadcastReceiver
{

	static private final String TAG = HeadsetReceiver.class.getSimpleName();

	@Override
	public void onReceive(Context context, Intent intent)
	{
    	Debug.d(TAG, "onReceive()");
    	try
    	{
    		String action = intent.getAction();
        	Debug.d(TAG, "onReceive() action : " + action);
        	if (action.equals(Intent.ACTION_HEADSET_PLUG))
        	{
        		int state = intent.getIntExtra("state", 0);
            	Debug.d(TAG, "onReceive() state : " + state);
            	AudioIOPlugin.setHeadphoneFlagAudioIOPlugin(0 < state);
        	}
        	Debug.d(TAG, "onReceive() finish");
    	}
    	catch (Exception e)
    	{
    	   	Debug.w(TAG, "onReceive() error : " + e);
    	   	e.printStackTrace();
    	}
	}

}
