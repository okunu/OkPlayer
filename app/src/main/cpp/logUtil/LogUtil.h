//
// Created by okunu on 2022/10/29.
//

#ifndef DEMO_LOGUTIL_H
#define DEMO_LOGUTIL_H

#include <android/log.h>
#include <string.h>

#define __FILE_NAME__ (strrchr(__FILE__, '/') + 1)

#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO,"okunu","[%s:%d] " fmt, __FILE_NAME__, __LINE__, ##__VA_ARGS__)

#endif //DEMO_LOGUTIL_H
