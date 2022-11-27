//
// Created by okunu on 2022/11/27.
//

#include "YuvSample.h"
#include "MyGlRenderContext.h"
#include "LogUtil.h"
#include <unistd.h>

YuvSample::YuvSample(): mVaoId(0), yTextureId(0), uTextureId(0), vTextureId(0) {
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

    //生成vao并绑定vao
    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    GLuint vboIds[2];
    glGenBuffers(2, vboIds);
    //绑定vbo[0]，GL_ARRAY_BUFFER类型的buffer，它只能有一个，所有三角形顶点和纹理就放在一起解析了
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)3);
    //绑定vbo[1]，GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(GL_NONE);

    prepareTexture();
}

void YuvSample::prepareTexture() {
    int width = 640;
    int height = 272;
    //处理纹理
    glGenTextures(1, &yTextureId);
    glBindTexture(GL_TEXTURE_2D, yTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,//细节基本 默认0
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图（这里就是只取一个亮度的颜色通道的意思）
                  width,//加载的纹理宽度。最好为2的次幂(这里对y分量数据当做指定尺寸算，但显示尺寸会拉伸到全屏？)
                  height,//加载的纹理高度。最好为2的次幂
                 0,//纹理边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图
                 GL_UNSIGNED_BYTE,//像素点存储的数据类型
                 NULL //纹理的数据（先不传）
    );
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenTextures(1, &uTextureId);
    glBindTexture(GL_TEXTURE_2D, uTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,//细节基本 默认0
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图（这里就是只取一个亮度的颜色通道的意思）
                 width/2,//加载的纹理宽度。最好为2的次幂(这里对y分量数据当做指定尺寸算，但显示尺寸会拉伸到全屏？)
                 height/2,//加载的纹理高度。最好为2的次幂
                 0,//纹理边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图
                 GL_UNSIGNED_BYTE,//像素点存储的数据类型
                 NULL //纹理的数据（先不传）
    );
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glGenTextures(1, &vTextureId);
    glBindTexture(GL_TEXTURE_2D, vTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,//细节基本 默认0
                 GL_LUMINANCE,//gpu内部格式 亮度，灰度图（这里就是只取一个亮度的颜色通道的意思）
                 width/2,//加载的纹理宽度。最好为2的次幂(这里对y分量数据当做指定尺寸算，但显示尺寸会拉伸到全屏？)
                 height/2,//加载的纹理高度。最好为2的次幂
                 0,//纹理边框
                 GL_LUMINANCE,//数据的像素格式 亮度，灰度图
                 GL_UNSIGNED_BYTE,//像素点存储的数据类型
                 NULL //纹理的数据（先不传）
    );
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void YuvSample::draw() {
    if (m_ProgramObj == 0) {
        return;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_ProgramObj);
    glBindVertexArray(mVaoId);

    //这里处理纹理
    int width = 640;
    int height = 272;
    glUniform1i(glGetUniformLocation(m_ProgramObj, "yTexture"), 0);
    glUniform1i(glGetUniformLocation(m_ProgramObj, "uTexture"), 1);
    glUniform1i(glGetUniformLocation(m_ProgramObj, "vTexture"), 2);

    unsigned char* buf[3] = {0};
    buf[0] = new unsigned char[width * height];
    buf[1] = new unsigned char[width * height / 4];
    buf[2] = new unsigned char[width * height / 4];
    auto manager = MyGlRenderContext::getInstance()->getAsset();
    AAsset* asset = AAssetManager_open(manager, "res/video1_640_272.yuv", AASSET_MODE_STREAMING);
    off_t length = AAsset_getLength(asset);
    long frameCount = length / (width * height * 3 / 2);
    LOGI("frameCount:%d", frameCount);
    for (int i = 0; i < frameCount; ++i) {
        int yread = AAsset_read(asset, buf[0], width * height);
        int uread = AAsset_read(asset, buf[1], width * height / 4);
        int vread = AAsset_read(asset, buf[2], width * height / 4);

        LOGI("i:%d", i);

        if (yread <= 0 || uread <= 0 || vread <= 0) {
            AAsset_close(asset);
            return;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, yTextureId);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
                        GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, uTextureId);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2,
                        GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[1]);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, vTextureId);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height/2,
                        GL_LUMINANCE, GL_UNSIGNED_BYTE, buf[2]);

        //注意，glDrawElements函数的第三个参数，本例中为GL_UNSIGNED_INT，必须要与indices数组的类型相同，如果不同，绘制会失败
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
//        usleep(20000);
    }

    glBindVertexArray(GL_NONE);
    AAsset_close(asset);
}