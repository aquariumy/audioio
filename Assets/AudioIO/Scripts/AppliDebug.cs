using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

public class AppliDebug : MonoBehaviour
{

	static public bool DEBUG = true;

	private const int DEBUG_MAX		= 16;
	static private string[] debug	= new string[DEBUG_MAX];
	static private int debugIndex	= 0;
	
	void OnGUI()
	{
		if (AppliDebug.DEBUG)
		{
			GUI.color		= Color.black;
			const int WL	= 2400;
			const int H		= 35;
			const int M		= 10;
			for (int i = 0; i < debugIndex; i++)
			{
				GUI.Label(new Rect(M, M + (H * i), WL, H), debug[i]);
			}
#if UNITY_EDITOR
			const int BW = 80;
			const int BH = 40;
			if (GUI.Button(new Rect(AppliPlugin.GetScreenWidth() - (BW + M) * 2, AppliPlugin.GetScreenHeight() - (BH + M), BW, BH), "Resume"))
			{
				Main.OnResume();
			}
			if (GUI.Button(new Rect(AppliPlugin.GetScreenWidth() - (BW + M) * 1, AppliPlugin.GetScreenHeight() - (BH + M), BW, BH), "Back"))
			{
				Main.OnClickBack();
			}
#endif
		}
	}

	static public void D(string tag, string mes)
	{
		if (AppliDebug.DEBUG)
		{
			string d = "[" + tag + "] " + mes;
			Debug.Log(d);
			P("D " + d);
		}
	}
	
	static public void W(string tag, string mes)
	{
		if (AppliDebug.DEBUG)
		{
			string w = "[" + tag + "] " + mes;
			Debug.LogWarning(w);
			P("W " + w);
		}
	}
	
	static private void P(string print)
	{
		AppliPlugin.Log(print);
		if (DEBUG_MAX <= debugIndex)
		{
			for (int i = 0; i < DEBUG_MAX - 1; i++)
			{
				debug[i] = debug[i + 1];
			}
			debugIndex--;
		}
		debug[debugIndex] = print;
		debugIndex++;
	}

}

