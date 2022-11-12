//
// Created by okunu on 2022/11/12.
//
#pragma once

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "MyGlRenderContext.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"okunu",__VA_ARGS__)


extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_nativecontext_NativeContext_setAssets(JNIEnv *env, jobject thiz,
                                                       jobject asset_manager) {
    AAssetManager* manager = AAssetManager_fromJava(env, asset_manager);
    auto context = MyGlRenderContext::getInstance();
    context->setAssetManager(manager);
}