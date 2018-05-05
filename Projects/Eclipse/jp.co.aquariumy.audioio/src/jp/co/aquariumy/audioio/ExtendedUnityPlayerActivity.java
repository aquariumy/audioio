package jp.co.aquariumy.audioio;

import com.unity3d.player.UnityPlayerActivity;

import android.hardware.usb.UsbManager;
import android.os.Bundle;

public class ExtendedUnityPlayerActivity extends UnityPlayerActivity
{

	static public UsbManager usbManager		= null;
	static public UsbReceiver usbReceiver	= new UsbReceiver();

	static private final String TAG = ExtendedUnityPlayerActivity.class.getSimpleName();

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
    	Debug.d(TAG, "onCreate()");
        super.onCreate(savedInstanceState);
        Unity.call("ACT_CREATE");
        usbManager = (UsbManager)getSystemService(USB_SERVICE);
    }

    @Override
    public void onStart()
    {
        super.onStart();
        Unity.call("ACT_START");
    }

    @Override
    public void onResume()
    {
        super.onResume();
        Unity.call("ACT_RESUME");
    }

    @Override
    public void onPause()
    {
        Unity.call("ACT_PAUSE");
        super.onPause();
    }

    @Override
    public void onStop()
    {
        Unity.call("ACT_STOP");
        super.onStop();
    }

    @Override
    public void onDestroy()
    {
        Unity.call("ACT_DESTROY");
        super.onDestroy();
    }

}

