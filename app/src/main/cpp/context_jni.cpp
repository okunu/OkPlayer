//
// Created by okunu on 2022/11/12.
//
#pragma once

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "MyGlRenderContext.h"
#include "JniHelper.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"okunu",__VA_ARGS__)

#ifdef JNI_VERSION_1_4
#define JNI_VER JNI_VERSION_1_4
#endif
// JDK 1.5 used JNI_VERSION 1.4!  But, just in case, keep it here.
#ifdef JNI_VERSION_1_5
#undef JNI_VER
#define JNI_VER JNI_VERSION_1_5
#endif
#ifdef JNI_VERSION_1_6
#undef JNI_VER
#define JNI_VER JNI_VERSION_1_6
#endif


extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_nativecontext_NativeContext_setAssets(JNIEnv *env, jobject thiz,
                                                       jobject asset_manager) {
    AAssetManager* manager = AAssetManager_fromJava(env, asset_manager);
    auto context = MyGlRenderContext::getInstance();
    context->setAssetManager(manager);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    JNIEnv* env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VER) != JNI_OK) {
        LOGI("JNI_OnLoad GetEnv failed");
        return JNI_EVERSION;
    }
    InitializeJniHelper(vm);
    return JNI_VER;
}