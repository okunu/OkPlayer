//
// Created by okunu on 2023/10/22.
//

#ifndef OKPLAYER_EGLDISPLAY_H
#define OKPLAYER_EGLDISPLAY_H

#include <android/native_window_jni.h>
#include <EGL/egl.h>

class EglDisplay {

public:
    EglDisplay(ANativeWindow* window);
    ~EglDisplay();
    int eglOpen();
    int eglClose();
    void swapBuffer();

private:
    ANativeWindow* window_;
    EGLDisplay eglDisplay_;
    EGLSurface eglSurface_;
    EGLContext eglContext_;
};

#endif //OKPLAYER_EGLDISPLAY_H
