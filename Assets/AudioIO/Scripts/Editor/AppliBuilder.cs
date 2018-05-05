using UnityEngine;
using UnityEditor;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;

public class AppliBuilder
{

	[UnityEditor.MenuItem("Tools/Build/Android")]
    static public void BuildAndroid()
	{
        EditorUserBuildSettings.SwitchActiveBuildTarget(BuildTarget.Android);
		PlayerSettings.defaultInterfaceOrientation				= UIOrientation.AutoRotation;
		PlayerSettings.allowedAutorotateToPortrait				= false;
		PlayerSettings.allowedAutorotateToPortraitUpsideDown	= false;
		PlayerSettings.allowedAutorotateToLandscapeLeft			= true;
		PlayerSettings.allowedAutorotateToLandscapeRight		= true;
		BuildPipeline.BuildPlayer(GetScenes(), "jp.co.aquariumy.audioio.apk", BuildTarget.Android, GetBuildOptions());
    }

	[UnityEditor.MenuItem("Tools/Build/iOS")]
    static public void BuildIOS()
	{
        EditorUserBuildSettings.SwitchActiveBuildTarget(BuildTarget.iOS);
		PlayerSettings.defaultInterfaceOrientation				= UIOrientation.AutoRotation;
		PlayerSettings.allowedAutorotateToPortrait				= false;
		PlayerSettings.allowedAutorotateToPortraitUpsideDown	= false;
		PlayerSettings.allowedAutorotateToLandscapeLeft			= true;
		PlayerSettings.allowedAutorotateToLandscapeRight		= true;
        BuildPipeline.BuildPlayer(GetScenes(), "iOS", BuildTarget.iOS, GetBuildOptions());
        File.Copy("Assets/AudioIO/Scripts/Editor/main.mm", "iOS/Classes/main.mm", true);
        var path    = "iOS/Unity-iPhone.xcodeproj/project.pbxproj";
        var text    = File.ReadAllText(path);
        text        = text.Replace("ENABLE_BITCODE = YES", "ENABLE_BITCODE = NO");
        File.WriteAllText(path, text);
		path		= "iOS/Info.plist";
		text		= File.ReadAllText(path);
		text   		= text.Replace("<dict>\n    ", "<dict>\n    <key>NSMicrophoneUsageDescription</key>\n    <string></string>\n    ");
		File.WriteAllText(path, text);
    }
	
	static private string[] GetScenes()
	{
		string[] result;
        var scenes = new List<string>();
        foreach (EditorBuildSettingsScene scene in EditorBuildSettings.scenes)
		{
			scenes.Add(scene.path);
        }
		result = scenes.ToArray();
		return result;
	}
	
	static private BuildOptions GetBuildOptions()
	{
		BuildOptions result = BuildOptions.None;
		if (AppliDebug.DEBUG)
		{
			result = BuildOptions.Development | BuildOptions.AllowDebugging;
		}
		return result;
	}
	
}
