#pragma once

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>
#include <android/bitmap.h>
#include "render/MyGlRenderContext.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"okunu",__VA_ARGS__)

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


extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_render_MyNativeRender_native_1init(JNIEnv *env, jobject thiz, jint type) {
    auto context = MyGlRenderContext::getInstance();
    context->initSampler(type);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_render_MyNativeRender_native_1uninit(JNIEnv *env, jobject thiz) {
    // TODO: implement native_uninit()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_render_MyNativeRender_native_1onSurfaceCreate(JNIEnv *env, jobject thiz, jobject surface) {
//    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    LOGI("jni surface created");
    auto context = MyGlRenderContext::getInstance();
    context->onSurfaceCreated();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_render_MyNativeRender_native_1onSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
    auto context = MyGlRenderContext::getInstance();
    LOGI("jni surface changed");
    context->onSurfaceChanged(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_render_MyNativeRender_native_1onDrawFrame(JNIEnv *env, jobject thiz) {
//    LOGI("jni surface draw");
    auto context = MyGlRenderContext::getInstance();
    context->onDrawFrame();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_render_MyNativeRender_changeDirection(JNIEnv *env, jobject thiz, jint direction) {
    auto context = MyGlRenderContext::getInstance();
    context->changeDirection(direction);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_render_MyNativeRender_setAngle(JNIEnv *env, jobject thiz, jfloat yaw,
                                                jfloat pitch) {
    auto context = MyGlRenderContext::getInstance();
    context->setAngle(yaw, pitch);
}