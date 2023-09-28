//
// Created by okunu on 2022/10/29.
//

#ifndef DEMO_LOGUTIL_H
#define DEMO_LOGUTIL_H

#include <android/log.h>

#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO,"okunu","[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#endif //DEMO_LOGUTIL_H
