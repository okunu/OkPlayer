//
// Created by okunu on 2022/11/27.
//

#include "YuvSample.h"
#include "MyGlRenderContext.h"
#include "LogUtil.h"
#include <unistd.h>

YuvSample::YuvSample(): texture{0}, VBO(0), EBO(0){
}

YuvSample::~YuvSample() noexcept {
}

void YuvSample::init() {
    if (m_ProgramObj != 0) {
        return;
    }
    LOGI("yuv sample init");
    auto verShader = MyGlRenderContext::getInstance()->getAssetResource("yuv/yuv.vert");
    auto fragShader = MyGlRenderContext::getInstance()->getAssetResource("yuv/yuv.frag");
    m_ProgramObj = GLUtils::createProgram(verShader.data(), fragShader.data(),
                                          m_VertexShader, m_FragmentShader);
    LOGI("m_ProgramObj = %d", m_ProgramObj);
    prepareData();
    prepareTexture();
}

void YuvSample::prepareData() {
    //这里包含了三角形顶点坐标，也包含了纹理顶点坐标，和解析颜色一样解析
    GLfloat verticesCoords[] = {
            -1.0f,  1.0f, 0.0f,  0.0f,  0.0f,// Position 0
            -1.0f, -1.0f, 0.0f,  0.0f,  1.0f,// Position 1
            1.0f, -1.0f, 0.0f,   1.0f,  1.0f,// Position 2
            1.0f,  1.0f, 0.0f,  1.0f,  0.0f // Position 3
    };

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(1, &VBO);
    //绑定VBO，GL_ARRAY_BUFFER类型的buffer，它只能有一个，所有三角形顶点和纹理就放在一起解析了
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //绑定EBO，GL_ELEMENT_ARRAY_BUFFER
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void YuvSample::prepareTexture() {
    int width = 640;
    int height = 272;
    //处理纹理
    glGenTextures(3, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
}

void YuvSample::draw() {
    if (m_ProgramObj == 0) {
        return;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //获取默认的display以及winsurface，绘制好一帧后swap，否则后面的帧无法显示
    //因为java端的glsurfaceview中已经处理好了相关的egl逻辑，如果我们要写就必须写全套，比较麻烦，先用这个代替
    auto display = eglGetCurrentDisplay();
    auto winSurface = eglGetCurrentSurface(EGL_READ);

    glUseProgram(m_ProgramObj);

    int width = 640;
    int height = 272;

    //使用vbo及ebo数据，不用每次绘制都解析
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //这段代码必须每次绘制的时候都调用，不指定的话，颜色会变绿，仔细想想也对，每次绘制的时候必须重新为每个纹理指定标号，不能只在初始化的时候指定一次
    glUniform1i(glGetUniformLocation(m_ProgramObj, "yTexture"), 0);
    glUniform1i(glGetUniformLocation(m_ProgramObj, "uTexture"), 1);
    glUniform1i(glGetUniformLocation(m_ProgramObj, "vTexture"), 2);

    unsigned char* buf[3] = {0};
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];

    AAssetManager* asManager = MyGlRenderContext::getInstance()->getAsset();
    AAsset* dataAsset = AAssetManager_open(asManager, "res/video1_640_272.yuv", AASSET_MODE_STREAMING);

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


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        eglSwapBuffers(display, winSurface);
        usleep(20000);
    }
    glBindVertexArray(GL_NONE);
    AAsset_close(dataAsset);
    delete[] buf[0];
    delete[] buf[1];
    delete[] buf[2];
}