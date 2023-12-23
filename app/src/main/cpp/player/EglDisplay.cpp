//
// Created by okunu on 2023/10/22.
//

#include "EglDisplay.h"
#include "LogUtil.h"

EglDisplay::EglDisplay(ANativeWindow *window): window_(window) {
    eglOpen();
}

EglDisplay::~EglDisplay() {
}

int EglDisplay::eglOpen() {
    if (window_ == nullptr) {
        return -1;
    }
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGI("egl display failed");
        return -1;
    }
    eglDisplay_ = display;

    //初始化egl，后两个参数为主次版本号
    EGLint majorVersion;
    EGLint minorVersion;
    if (eglInitialize(display, &majorVersion, &minorVersion) != EGL_TRUE){
        LOGI("egl init failed");
        return -1;
    }
    //surface配置，可以理解为窗口
    EGLConfig eglConfig;
    EGLint configNum;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };
    if (eglChooseConfig(display, configSpec, &eglConfig, 1, &configNum) != EGL_TRUE) {
        LOGI("egl choose config failed");
        return -1;
    }

    const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        LOGI("create egl context failed");
        return -1;
    }
    eglContext_ = context;

    EGLint eglFormat;
    EGLBoolean success = eglGetConfigAttrib(display, eglConfig, EGL_NATIVE_VISUAL_ID, &eglFormat);
    if (!success) {
        LOGI("eglGetConfigAttrib failed");
        return -1;
    }

    EGLSurface winSurface = eglCreateWindowSurface(display, eglConfig, window_, 0);
    if (winSurface == EGL_NO_SURFACE) {
        LOGI("create window surface failed");
        return -1;
    }
    eglSurface_ = winSurface;

//    if (EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context)) {
//        LOGI("eglmakecurrent failed");
//        return -1;
//    }
    return 0;
}

void EglDisplay::bindCurrent() {
    if (EGL_TRUE != eglMakeCurrent(eglDisplay_, eglSurface_, eglSurface_, eglContext_)) {
        LOGI("eglmakecurrent failed");
    }
}

void EglDisplay::swapBuffer() {
    eglSwapBuffers(eglDisplay_, eglSurface_);
}

int EglDisplay::eglClose() {
    EGLBoolean success = eglDestroySurface(eglDisplay_, eglSurface_);
    if (!success) {
        LOGI("eglDestroySurface fail");
    }

    success = eglDestroyContext(eglDisplay_, eglContext_);
    if (!success) {
        LOGI("eglDestroyContext fail");
    }

    success = eglTerminate(eglDisplay_);
    if (!success) {
        LOGI("eglTerminate fail");
    }

    eglSurface_ = NULL;
    eglContext_ = NULL;
    eglDisplay_ = NULL;
}