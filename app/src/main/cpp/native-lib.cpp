#include <jni.h>
#include <string>
extern "C"{
#include <libavutil/avutil.h>
}

const char* getVer() {
    return av_version_info();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_ou_demo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    return env->NewStringUTF(getVer());
}