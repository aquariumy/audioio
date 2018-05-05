package jp.co.aquariumy.audioio;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;

import com.unity3d.player.UnityPlayer;

import android.annotation.SuppressLint;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.hardware.usb.UsbDevice;
import android.os.Build;
import android.view.Display;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ProgressBar;

public class UnityPlugin
{

	static private final String TAG = UnityPlugin.class.getSimpleName();

	static private Display display						= null;
	static private FrameLayout frameLayout				= null;
	static private View inputGuardView					= null;
	static private ProgressBar progressBar				= null;

	@SuppressLint("DefaultLocale")
	static public void createPlugin()
	{
    	Debug.d(TAG, "createPlugin()");
    	// display
        WindowManager wm	= (WindowManager)UnityPlayer.currentActivity.getSystemService(Context.WINDOW_SERVICE);
        display				= wm.getDefaultDisplay();
        // Views
    	UnityPlayer.currentActivity.runOnUiThread(new Runnable()
    	{
			@SuppressLint("SetJavaScriptEnabled")
			public void run()
    		{
    	    	Debug.d(TAG, "createPlugin() start");
    	    	Debug.d(TAG, "createPlugin() BRAND        : " + Build.BRAND);
    	    	Debug.d(TAG, "createPlugin() MANUFACTURER : " + Build.MANUFACTURER);
    	    	Debug.d(TAG, "createPlugin() MODEL        : " + Build.MODEL);
    	        // frameLayout
    			frameLayout = new FrameLayout(UnityPlayer.currentActivity);
    			UnityPlayer.currentActivity.addContentView(frameLayout, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
    			frameLayout.setFocusable(true);
    			frameLayout.setFocusableInTouchMode(true);
    			if (Build.VERSION_CODES.HONEYCOMB <= Build.VERSION.SDK_INT)
    			{
        			// inputGuardView
        			inputGuardView = new View(UnityPlayer.currentActivity);
        			inputGuardView.setVisibility(View.GONE);
        			inputGuardView.setBackgroundColor(Color.BLACK);
        			inputGuardView.setAlpha(0.5f);
        			frameLayout.addView(inputGuardView, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT, Gravity.NO_GRAVITY));
    			}
    			// progressBar
    			int sw		= getScreenWidthPlugin();
    			int sh		= getScreenHeightPlugin();
    			int style	= android.R.attr.progressBarStyle;
    			if ((Build.VERSION.SDK_INT < Build.VERSION_CODES.HONEYCOMB) && (0 <= Build.MANUFACTURER.toUpperCase().indexOf("PANASONIC")) && (Build.MODEL.indexOf("P-") == 0))
    			{
    				// OS 2.3 以下の P 系機種はスタイル変更
    				style = android.R.attr.progressBarStyleHorizontal;
    			}
    			progressBar = new ProgressBar(UnityPlayer.currentActivity, null, style);
    			progressBar.setIndeterminate(true);
    			progressBar.setVisibility(View.GONE);
    			frameLayout.addView(progressBar, new FrameLayout.LayoutParams(sw / 16, sh / 16, Gravity.CENTER));
    	    	Debug.d(TAG, "createPlugin() end");
    		}
    	});
	}

	static public void destroyPlugin()
	{
    	Debug.d(TAG, "destroyPlugin()");
        // Views
    	UnityPlayer.currentActivity.runOnUiThread(new Runnable()
    	{
    		public void run()
    		{
    	    	Debug.d(TAG, "destroyPlugin() start");
    	    	if (inputGuardView != null)
    	    	{
    	        	frameLayout.removeView(inputGuardView);
    	        	inputGuardView = null;
    	    	}
    	    	if (progressBar != null)
    	    	{
    	        	frameLayout.removeView(progressBar);
    	        	progressBar = null;
    	    	}
    	    	Debug.d(TAG, "destroyPlugin() end");
    		}
    	});
	}

	static public String getVersionPlugin()
    {
		String result = "";
		try
		{
			result = UnityPlayer.currentActivity.getPackageManager().getPackageInfo(UnityPlayer.currentActivity.getPackageName(), 0).versionName;
        }
        catch (Exception e)
        {
        	//
        }
        return result;
    }

    @SuppressWarnings("deprecation")
	static public int getScreenWidthPlugin()
    {
        return display.getWidth();
    }

    @SuppressWarnings("deprecation")
	static public int getScreenHeightPlugin()
    {
        return display.getHeight();
    }

    static public void setVisibleLoadingViewPlugin(final String option)
    {
    	Debug.d(TAG, "setVisibleLoadingViewPlugin() option : " + option);
    	UnityPlayer.currentActivity.runOnUiThread(new Runnable()
    	{
    		public void run()
    		{
    	    	Debug.d(TAG, "setVisibleLoadingViewPlugin() start");
    	    	if (inputGuardView != null)
    	    	{
        	    	inputGuardView.setVisibility(option.equals("transparent") ? View.GONE : View.VISIBLE);
    	    	}
    	    	progressBar.setVisibility(View.VISIBLE);
    	    	Debug.d(TAG, "setVisibleLoadingViewPlugin() end");
    		}
    	});
    }

    static public void setInvisibleLoadingViewPlugin()
    {
    	Debug.d(TAG, "setInvisibleLoadingViewPlugin()");
    	UnityPlayer.currentActivity.runOnUiThread(new Runnable()
    	{
    		public void run()
    		{
    	    	Debug.d(TAG, "setInvisibleLoadingViewPlugin() start");
    	    	if (inputGuardView != null)
    	    	{
        	    	inputGuardView.setVisibility(View.GONE);
    	    	}
    	    	progressBar.setVisibility(View.GONE);
    	    	Debug.d(TAG, "setInvisibleLoadingViewPlugin() end");
    		}
    	});
    }

    static public int getVersionSdkIntPlugin()
    {
    	return Build.VERSION.SDK_INT;
    }

    static public String getManufacturerPlugin()
    {
    	return Build.MANUFACTURER.toUpperCase(Locale.ENGLISH);
    }

    static public String getModelPlugin()
    {
    	return Build.MODEL.toUpperCase(Locale.ENGLISH);
    }

    static public void requestUsbPermission()
	{
    	Debug.d(TAG, "requestUsbPermission()");
		HashMap<String, UsbDevice> usbDeviceList	= ExtendedUnityPlayerActivity.usbManager.getDeviceList();
		Iterator<UsbDevice> usbDeviceIterator		= usbDeviceList.values().iterator();
		boolean isConnecting		= false;
		UsbDevice connectingDevice	= null;
		while (usbDeviceIterator.hasNext())
		{
			UsbDevice usbDevice = usbDeviceIterator.next();
			Debug.d(TAG, "requestUsbPermission() usb-device-id       : " + usbDevice.getDeviceId());
			Debug.d(TAG, "requestUsbPermission() usb-device-name     : " + usbDevice.getDeviceName());
			Debug.d(TAG, "requestUsbPermission() usb-device-protocol : " + usbDevice.getDeviceProtocol());
			Debug.d(TAG, "requestUsbPermission() usb-product-id      : " + usbDevice.getProductId());
			Debug.d(TAG, "requestUsbPermission() usb-product-name    : " + usbDevice.getProductName());
			Debug.d(TAG, "requestUsbPermission() usb-vendor-id       : " + usbDevice.getVendorId());
			//if (0 <= usbDevice.getProductName().indexOf("iRig"))
			{
				isConnecting		= true;
				connectingDevice	= usbDevice;
				break;
			}
		}
		if (isConnecting)
		{
			IntentFilter intentFilter = new IntentFilter(UsbReceiver.ACTION_USB_PERMISSION);
			UnityPlayer.currentActivity.registerReceiver(ExtendedUnityPlayerActivity.usbReceiver, intentFilter);
			PendingIntent pendingIntent	= PendingIntent.getBroadcast(UnityPlayer.currentActivity, 0, new Intent(UsbReceiver.ACTION_USB_PERMISSION), 0);
			ExtendedUnityPlayerActivity.usbManager.requestPermission(connectingDevice, pendingIntent);
		}
		else
		{
			Unity.audioio("INIT,");
		}
	}

}