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
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <unistd.h>
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

void initEgl(JNIEnv *env, jobject thiz, jobject surface, EGLDisplay* display, EGLSurface* winSurface) {
    //获取原始窗口
    ANativeWindow *nwin = ANativeWindow_fromSurface(env, surface);
    //获取display
    *display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (*display == EGL_NO_DISPLAY) {
        LOGI("egl display failed");
        return;
    }
    //初始化egl，后两个参数为主次版本号
    if (eglInitialize(*display, 0, 0) != EGL_TRUE){
        LOGI("egl init failed");
        return;
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
    if (eglChooseConfig(*display, configSpec, &eglConfig, 1, &configNum) != EGL_TRUE) {
        LOGI("egl choose config failed");
        return;
    }

    *winSurface = eglCreateWindowSurface(*display, eglConfig, nwin, 0);
    if (*winSurface == EGL_NO_SURFACE) {
        LOGI("create window surface failed");
        return;
    }
    const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    EGLContext context = eglCreateContext(*display, eglConfig, EGL_NO_CONTEXT, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        LOGI("create egl context failed");
        return;
    }
    if (EGL_TRUE != eglMakeCurrent(*display, *winSurface, *winSurface, context)) {
        LOGI("eglmakecurrent failed");
        return;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_opengl_YuvPlayer_drawTriangle(JNIEnv *env, jobject thiz, jobject surface) {
    EGLDisplay display;
    EGLSurface winSurface;
    initEgl(env, thiz, surface, &display, &winSurface);

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

static const char* yuvFrag = "#version 300 es\n"
                             "precision mediump float;\n"
                             "//输入的yuv三个纹理\n"
                             "uniform sampler2D yTexture;//采样器\n"
                             "uniform sampler2D uTexture;//采样器\n"
                             "uniform sampler2D vTexture;//采样器\n"
                             "out vec4 FragColor;\n"
                             "//纹理坐标\n"
                             "in vec2 vTextCoord;\n"
                             "void main() {\n"
                             "    //采样到的yuv向量数据\n"
                             "    vec3 yuv;\n"
                             "    //yuv转化得到的rgb向量数据\n"
                             "    vec3 rgb;\n"
                             "    //分别取yuv各个分量的采样纹理\n"
                             "    yuv.x = texture(yTexture, vTextCoord).r;\n"
                             "    yuv.y = texture(uTexture, vTextCoord).g - 0.5;\n"
                             "    yuv.z = texture(vTexture, vTextCoord).b - 0.5;\n"
                             "    //yuv转化为rgb\n"
                             "    rgb = mat3(\n"
                             "        1.0, 1.0, 1.0,\n"
                             "        0.0, -0.183, 1.816,\n"
                             "        1.540, -0.459, 0.0\n"
                             "    ) * yuv;\n"
                             "    //gl_FragColor是OpenGL内置的\n"
                             "    FragColor = vec4(rgb, 1.0);\n"
                             "}";


static const char* yuvVert = "#version 300 es\n"
                             "layout(location = 0) in vec4 a_position;\n"
                             "layout(location = 1) in vec2 a_texCoord;\n"
                             "out vec2 vTextCoord;\n"
                             "void main()\n"
                             "{\n"
                             "   gl_Position = a_position;\n"
                             "   vTextCoord = a_texCoord;\n"
                             "}";

extern "C"
JNIEXPORT void JNICALL
Java_com_ou_demo_opengl_YuvPlayer_drawYuv(JNIEnv *env, jobject thiz, jobject surface,
                                          jobject manager) {
    EGLDisplay display;
    EGLSurface winSurface;
    initEgl(env, thiz, surface, &display, &winSurface);

    GLint vsh = initShader(yuvVert, GL_VERTEX_SHADER);
    GLint fsh = initShader(yuvFrag, GL_FRAGMENT_SHADER);

    GLint program = glCreateProgram();
    if (program == 0) {
        LOGI("glCreateProgram failed");
        return;
    }
    glAttachShader(program, vsh);
    glAttachShader(program, fsh);

    glLinkProgram(program);
    GLint status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == 0) {
        LOGI("glLinkProgram failed");
        return;
    }
    glUseProgram(program);
    static float ver[] = {
            -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f
    };
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ver);
    glEnableVertexAttribArray(0);

    //加入纹理坐标数据
    static float fragment[] = {
            0.0f,  0.0f,        // TexCoord 0
            0.0f,  1.0f,        // TexCoord 1
            1.0f,  1.0f,        // TexCoord 2
            1.0f,  0.0f
    };
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, fragment);
    glEnableVertexAttribArray(1);

    GLushort indices[] = {0, 1, 2, 0, 2, 3};

    int width = 576;
    int height = 1024;

    glUniform1i(glGetUniformLocation(program, "yTexture"), 0);
    glUniform1i(glGetUniformLocation(program, "uTexture"), 1);
    glUniform1i(glGetUniformLocation(program, "vTexture"), 2);

    GLuint texture[3] = {0};
    glGenTextures(3, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    //缩小的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //放大的过滤器
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width/2, height/2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width/2, height/2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, NULL);

    unsigned char* buf[3] = {0};
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];

    AAssetManager* asManager = AAssetManager_fromJava(env, manager);
    AAsset* dataAsset = AAssetManager_open(asManager, "res/demo_576_1024.yuv", AASSET_MODE_STREAMING);

    off_t bufferSize = AAsset_getLength(dataAsset);
    long frameCount = bufferSize / (width * height * 3 / 2);
    LOGI("frameCount%d", frameCount);

    for (int i = 0; i < frameCount; ++i) {
        int bufYRead = AAsset_read(dataAsset, buf[0], width * height);
        int bufURead = AAsset_read(dataAsset, buf[1], width * height / 4);
        int bufVRead = AAsset_read(dataAsset, buf[2], width * height / 4);

        if (bufYRead <= 0 || bufURead <= 0 || bufVRead <= 0) {
            AAsset_close(dataAsset);
            return;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                        GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2,
                        GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[1]);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2,
                        GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[2]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

        eglSwapBuffers(display, winSurface);
        usleep(20000);
    }
    AAsset_close(dataAsset);
}