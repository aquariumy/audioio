package jp.co.aquariumy.audioio;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbManager;

public class UsbReceiver extends BroadcastReceiver
{

	static public final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";

	static private final String TAG = UsbReceiver.class.getSimpleName();

	@Override
	public void onReceive(Context context, Intent intent)
	{
		Debug.d(TAG, "onReceive()");
		String action = intent.getAction();
		if (action.equals(ACTION_USB_PERMISSION))
		{
			if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false))
			{
				Debug.d(TAG, "onReceive() yes");
				Unity.audioio("INIT,");
			}
			else
			{
				Debug.d(TAG, "onReceive() no");
			}
        }
	}

}
