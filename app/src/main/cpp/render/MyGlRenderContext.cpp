//
// Created by okunu on 2022/10/29.
//

#include "MyGlRenderContext.h"
#include "TextureSample.h"
#include "RenderType.h"
#include "VaoSample.h"
#include "YuvSample.h"
#include "UniformSample.h"

MyGlRenderContext* MyGlRenderContext::mInstance = nullptr;

MyGlRenderContext::MyGlRenderContext(): pixel(nullptr), mSample(nullptr) {
}

MyGlRenderContext::~MyGlRenderContext() {
    delete mSample;
    pixel = nullptr;
}

MyGlRenderContext* MyGlRenderContext::getInstance() {
    if (mInstance == nullptr) {
        mInstance = new MyGlRenderContext();
    }
    return mInstance;
}

void MyGlRenderContext::releaseInstance() {
    if (mInstance) {
        delete mInstance;
        mInstance = nullptr;
    }
}

void MyGlRenderContext::onSurfaceCreated() {
//    configEgl(nwin);
    LOGI("msample = %p", mSample);
    mSample->init();
}

void MyGlRenderContext::onSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
}

void MyGlRenderContext::onDrawFrame() {
    mSample->draw();
//    swapBuffer();
}

void MyGlRenderContext::setBitmapData(void *data, int width, int height) {
    if (mSample->hasTextureSample() && data != nullptr) {
        mSample->initTextureData(data, width, height);
    }
}

void MyGlRenderContext::setAssetManager(AAssetManager *manager) {
    this->manager = manager;
}

void MyGlRenderContext::initSampler(int type) {
    mSample = nullptr;
    if (type == RenderType::Triangle) {
        mSample = new TriangleSample();
    } else if (type == RenderType::Texture) {
        mSample = new TextureSample();
    } else if (type == RenderType::Vao) {
        mSample = new VaoSample();
    } else if (type == RenderType::Yuv) {
        mSample = new YuvSample();
    } else if (type == RenderType::Uniform) {
        mSample = new UniformSample();
    }
}

AAssetManager* MyGlRenderContext::getAsset() {
    if (manager == nullptr) {
        LOGI("manager is null");
        return nullptr;
    }
    return manager;
}

std::string MyGlRenderContext::getAssetResource(const std::string &path) {
    if (manager == nullptr) {
        LOGI("manager is null");
        return nullptr;
    }
    auto asset = AAssetManager_open(manager, path.data(), AASSET_MODE_UNKNOWN);
    if (!asset) {
        LOGI("AssetUtil", "loadTextAsset open error");
        return nullptr;
    }
    off_t len = AAsset_getLength(asset);
    char *buffer = new char[len + 1];
    uint32_t num = AAsset_read(asset, buffer, len);
    AAsset_close(asset);
    if (num != len) {
        LOGI("AssetUtil", "loadTextAsset read error");
        delete[] buffer;
        return nullptr;
    }
    buffer[len] = '\0';
    std::string result(buffer);
    delete[] buffer;
    return result;
}

void MyGlRenderContext::swapBuffer() {
    eglSwapBuffers(display, winSurface);
}

void MyGlRenderContext::configEgl(ANativeWindow *nwin) {
    //获取display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGI("egl display failed");
        return;
    }
    //初始化egl，后两个参数为主次版本号
    if (eglInitialize(display, 0, 0) != EGL_TRUE){
        LOGI("egl init failed");
        return;
    }
    //surface配置，可以理解为窗口
    EGLConfig eglConfig;
    EGLint configNum;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 4,
            EGL_GREEN_SIZE, 4,
            EGL_BLUE_SIZE, 4,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };
    if (eglChooseConfig(display, configSpec, &eglConfig, 1, &configNum) != EGL_TRUE) {
        LOGI("egl choose config failed");
        return;
    }

    winSurface = eglCreateWindowSurface(display, eglConfig, nwin, 0);
    if (winSurface == EGL_NO_SURFACE) {
        LOGI("create window surface failed");
        return;
    }
    const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(display, eglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        LOGI("create egl context failed");
        return;
    }
    if (EGL_TRUE != eglMakeCurrent(display, winSurface, winSurface, context)) {
        LOGI("eglmakecurrent failed");
        return;
    }
}