#include <windows.h>
#include <jni.h>

JNIEXPORT void JNICALL Java_com_ememisya_llamacpp_NativeLibraryLoader_setDllDirectory
  (JNIEnv *env, jclass cls, jstring path) {

    const jchar *chars = (*env)->GetStringChars(env, path, NULL);
    SetDllDirectoryW((LPCWSTR)chars);
    (*env)->ReleaseStringChars(env, path, chars);
}
