//
// Created by okunu on 2022/12/10.
//

#include "JniHelper.h"
#include "LogUtil.h"

JavaVM* s_jni_vm = nullptr;

void InitializeJniHelper(JavaVM* vm) {
    s_jni_vm = vm;
}

JNIEnv* GetJniEnv(){
    if (s_jni_vm == NULL) return NULL;
    JNIEnv *jni_env = NULL;
    int status = s_jni_vm->GetEnv((void **)&jni_env, JNI_VERSION_1_6);
    //LOGI("status = %d", status);
    if (status == JNI_EDETACHED || jni_env == NULL) {
        status = s_jni_vm->AttachCurrentThread(&jni_env, NULL);
        if (status < 0) {
            jni_env = NULL;
        }
    }
    return jni_env;
}