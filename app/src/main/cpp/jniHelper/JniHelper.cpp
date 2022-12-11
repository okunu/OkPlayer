//
// Created by okunu on 2022/12/10.
//

#include "JniHelper.h"

JavaVM* s_jni_vm = nullptr;

void InitializeJniHelper(JavaVM* vm) {
    s_jni_vm = vm;
}

JNIEnv* GetJniEnv(){
    JNIEnv* env = nullptr;
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_4;
    args.name = "pthread-test";
    args.group = NULL;
    s_jni_vm->AttachCurrentThread(&env, &args);
    s_jni_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_4);
    return env;
}