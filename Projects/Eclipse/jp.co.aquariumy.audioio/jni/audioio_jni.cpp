
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "audioio_define.h"
#include "audioio_dll.h"
#include "audioio.h"
#include "audioio_jni.h"

#ifdef DEBUG
#   define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "AudioIOJni", __VA_ARGS__)
#   define LOGW(...) __android_log_print(ANDROID_LOG_WARN,  "AudioIOJni", __VA_ARGS__)
#else
#   define LOGD(...)
#   define LOGW(...)
#endif

#define NUM_ARRAY_ELEMENTS(p) ((int)(sizeof(p) / sizeof(p[0])))
#define JAVA_CLASS "jp/co/aquariumy/audioio/AudioIOPlugin"

static JavaVM *javaVM;

static JNINativeMethod methods[] =
{
    {"create",              "()V",                          (void *)createPlugin},
    {"destroy",             "()V",                          (void *)destroyPlugin},
    {"getSamplesPerBuffer", "()I",                          (void *)getSamplesPerBufferPlugin},
    {"setSamplesPerBuffer", "(I)V",                         (void *)setSamplesPerBufferPlugin},
    {"getInputVolume",      "()F",                          (void *)getInputVolumePlugin},
    {"setInputVolume",      "(F)V",                         (void *)setInputVolumePlugin},
    {"getOutputVolume",     "()F",                          (void *)getOutputVolumePlugin},
    {"setOutputVolume",     "(F)V",                         (void *)setOutputVolumePlugin},
    {"getMessage",          "()Ljava/lang/String;",         (void *)getMessagePlugin},
    {"setHeadphoneFlag",    "(Z)V",                         (void *)setHeadphoneFlagPlugin},
};

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    LOGD("JNI_OnLoad()");
    jint result = JNI_ERR;
    javaVM      = vm;
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) == JNI_OK)
    {
        if (registerNativeMethods(env, JAVA_CLASS, methods, NUM_ARRAY_ELEMENTS(methods)) == 0)
        {
            LOGD("JNI_OnLoad() JNI_VERSION_1_4");
            result = JNI_VERSION_1_4;
        }
        else
        {
            LOGW("JNI_OnLoad() error B");
        }
    }
    else
    {
        LOGW("JNI_OnLoad() error A");
    }
    return result;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    LOGD("JNI_OnUnload()");
}

static int registerNativeMethods(JNIEnv *env, const char *class_name, JNINativeMethod *methods, int num_methods)
{
    LOGD("registerNativeMethods()");
    int result   = -1;
    jclass clazz = env->FindClass(class_name);
    if (clazz)
    {
        result = env->RegisterNatives(clazz, methods, num_methods);
        LOGD("registerNativeMethods() result : %d", result);
        if (result < 0)
        {
            LOGW("registerNativeMethods() error class_name : %s", class_name);
        }
        env->DeleteLocalRef(clazz);
    }
    else
    {
        LOGW("registerNativeMethods() not found class_name : %s", class_name);
    }
    return result;
}

static void createPlugin(JNIEnv *env, jobject thiz)
{
    LOGD("createPlugin()");
	createAudioIOPlugin();
}

static void destroyPlugin(JNIEnv *env, jobject thiz)
{
    LOGD("destroyPlugin()");
	destroyAudioIOPlugin();
}

static jint getSamplesPerBufferPlugin(JNIEnv *env, jobject thiz)
{
    LOGD("getSamplesPerBufferPlugin()");
    return (jint)getSamplesPerBufferAudioIOPlugin();
}

static void setSamplesPerBufferPlugin(JNIEnv *env, jobject thiz, jint value)
{
    LOGD("setSamplesPerBufferPlugin()");
    setSamplesPerBufferAudioIOPlugin((int)value);
}

static jfloat getInputVolumePlugin(JNIEnv *env, jobject thiz)
{
    LOGD("getInputVolumePlugin()");
    return (jfloat)getInputVolumeAudioIOPlugin();
}

static void setInputVolumePlugin(JNIEnv *env, jobject thiz, jfloat value)
{
    LOGD("setInputVolumePlugin()");
    setInputVolumeAudioIOPlugin((float)value);
}

static jfloat getOutputVolumePlugin(JNIEnv *env, jobject thiz)
{
    LOGD("getOutputVolumePlugin()");
	return (jfloat)getOutputVolumeAudioIOPlugin();
}

static void setOutputVolumePlugin(JNIEnv *env, jobject thiz, jfloat value)
{
    LOGD("setOutputVolumePlugin()");
	setOutputVolumeAudioIOPlugin((float)value);
}

static jstring getMessagePlugin(JNIEnv *env, jobject thiz)
{
    return env->NewStringUTF(getMessageAudioIOPlugin());
}

static void setHeadphoneFlagPlugin(JNIEnv *env, jobject thiz, jboolean flag)
{
    LOGD("setHeadphoneFlagPlugin()");
    setHeadphoneFlagAudioIOPlugin(flag == JNI_TRUE ? 1 : 0);
}

