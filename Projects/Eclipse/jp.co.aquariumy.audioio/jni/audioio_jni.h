
#include <jni.h>

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved);

static int registerNativeMethods(JNIEnv *env, const char *class_name, JNINativeMethod *methods, int num_methods);
static void createPlugin(JNIEnv *env, jobject thiz);
static void destroyPlugin(JNIEnv *env, jobject thiz);
static jint getSamplesPerBufferPlugin(JNIEnv *env, jobject thiz);
static void setSamplesPerBufferPlugin(JNIEnv *env, jobject thiz, jint value);
static jfloat getInputVolumePlugin(JNIEnv *env, jobject thiz);
static void setInputVolumePlugin(JNIEnv *env, jobject thiz, jfloat value);
static jfloat getOutputVolumePlugin(JNIEnv *env, jobject thiz);
static void setOutputVolumePlugin(JNIEnv *env, jobject thiz, jfloat value);
static jstring getMessagePlugin(JNIEnv *env, jobject thiz);
static void setHeadphoneFlagPlugin(JNIEnv *env, jobject thiz, jboolean flag);

