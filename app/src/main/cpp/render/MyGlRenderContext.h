//
// Created by okunu on 2022/10/29.
//

#ifndef DEMO_MYGLRENDERCONTEXT_H
#define DEMO_MYGLRENDERCONTEXT_H

#include "../logUtil/LogUtil.h"
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "TriangleSample.h"
#include <android/asset_manager.h>

class MyGlRenderContext {

private:
    MyGlRenderContext();
    ~MyGlRenderContext();

public:
    void onSurfaceCreated();
    void configEgl(ANativeWindow* window);
    void onSurfaceChanged(int width, int height);
    void onDrawFrame();
    void swapBuffer();
    void setAssetManager(AAssetManager* manager);
    std::string getAssetResource(const std::string& path);
    void getBitmap(const char* path, void** data, int& width, int& height);
    void initSampler(int type);
    AAssetManager* getAsset();
    int getWidth() {return mWidth;}
    int getHeight() { return mHeight;}
    void changeDirection(int direction);
    void setAngle(float yaw, float pitch);

    static MyGlRenderContext* getInstance();
    static void releaseInstance();

private:
    static MyGlRenderContext *mInstance;
    EGLDisplay display;
    EGLSurface winSurface;
    BaseSample* mSample;
    AAssetManager* manager;
    int mWidth;
    int mHeight;
};

#endif //DEMO_MYGLRENDERCONTEXT_H
