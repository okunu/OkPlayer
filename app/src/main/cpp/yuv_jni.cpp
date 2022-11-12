//
// Created by okunu on 2022/11/12.
//
#pragma once

#include <jni.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <string>
#include <android/bitmap.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"okunu",__VA_ARGS__)

static const char *fragSimpleShape =
        "  #version 300 es\n"
        "        precision\n"
        "        mediump float;\n"
        "\n"
        "        in\n"
        "        vec4 vTextColor;//输入的颜色\n"
        "        out vec4 FragColor;\n"

        "        in\n"
        "        vec4 vPosition;//输入的坐标\n"

        "        void main() {\n"
        "            //gl_FragColor是OpenGL内置的\n"
        "            FragColor = vec4(vPosition.x ,vPosition.y ,vPosition.z,1.0);\n"
        "        }";

static const char *vertexSimpleShape =
        "        #version 300 es\n"
        "        layout (location = 0) \n"
        "        in vec4 aPosition;//输入的顶点坐标，会在程序指定将数据输入到该字段\n"
        "\n"
        "        out\n"
        "        vec4 vTextColor;//输出的颜色\n"

        "        out\n"
        "        vec4 vPosition;//输出的坐标\n"
        "\n"
        "        void main() {\n"
        "            //直接把传入的坐标值作为传入渲染管线。gl_Position是OpenGL内置的\n"
        "            gl_Position = aPosition;\n"
        "            vPosition = aPosition;\n"
        "            gl_PointSize = 50.0;\n"
        //        "            vTextColor = vec4(aPosition.x ,aPosition.y ,aPosition.z,1.0);\n"
        "        }";

GLint initShader(const char* source, int type);

GLint initShader(const char* source, GLint type) {
    GLint sh = glCreateShader(type);
    if (sh == 0) {
        LOGI("glcreate shader %d failed", type);
        return 0;
    }
    glShaderSource(sh, 1, &source, 0);
    glCompileShader(sh);
    GLint status;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        LOGI("glcompile shader %d failed", type);
        LOGI("source %s", source);
        auto* infoLog = new GLchar [512];
        GLsizei length;
        glGetShaderInfoLog(sh, 512, &length, infoLog);
        LOGI("error share compile failed: %s", infoLog);
        delete[] infoLog;
        return 0;
    }
    LOGI("glcompile share %d success", type);
    return sh;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_opengl_YuvPlayer_drawTriangle(JNIEnv *env, jobject thiz, jobject surface) {
    //获取原始窗口
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    //获取display
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
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

    EGLSurface winSurface = eglCreateWindowSurface(display, eglConfig, nwin, 0);
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



    GLint vsh = initShader(vertexSimpleShape, GL_VERTEX_SHADER);
    GLint fsh = initShader(fragSimpleShape, GL_FRAGMENT_SHADER);

    GLint program = glCreateProgram();
    if (program == 0) {
        LOGI("glcreate program failed");
        return;
    }

    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0) {
        LOGI("link program failed");
        return;
    }
    LOGI("link program success");

    glUseProgram(program);

    static float triangleVer[] = {
            0.8f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.8f, 0.0f,
    };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, triangleVer);
    glEnableVertexAttribArray(0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    eglSwapBuffers(display, winSurface);
    glDeleteProgram(program);
}

