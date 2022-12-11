//
// Created by okunu on 2022/12/10.
//

#ifndef DEMO_JNIHELPER_H
#define DEMO_JNIHELPER_H

#include <jni.h>

void InitializeJniHelper(JavaVM* vm);
JNIEnv* GetJniEnv();

#endif //DEMO_JNIHELPER_H
