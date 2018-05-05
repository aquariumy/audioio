package jp.co.aquariumy.audioio;

import com.unity3d.player.UnityPlayer;

public class Unity
{

	static public void call(String message)
	{
        Unity.sendMessage(UnitySendMessageType.CALL, message);
	}

	static public void audioio(String message)
	{
        Unity.sendMessage(UnitySendMessageType.AUDIOIO, message);
	}

	static public void d(String message)
	{
        Unity.sendMessage(UnitySendMessageType.LOG_D, message);
	}

	static public void w(String message)
	{
        Unity.sendMessage(UnitySendMessageType.LOG_W, message);
	}

	static private void sendMessage(UnitySendMessageType type, String message)
	{
		UnityPlayer.UnitySendMessage("Receiver", type.getMethod(), message);
	}

}

