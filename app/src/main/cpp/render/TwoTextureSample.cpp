//
// Created by okunu on 2022/12/10.
//

#include "TwoTextureSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"

TwoTextureSample::TwoTextureSample(): mVaoId(0), mLyfTextureId(0), mNsTextureId(0) {
}

void TwoTextureSample::init() {
    LOGI("TwoTextureSample init");
    if (m_ProgramObj != 0) {
        return;
    }
    auto vShaderStr = MyGlRenderContext::getInstance()->getAssetResource("twotexture/twotexture.vert");
    LOGI("init----vShaderStr = %s", vShaderStr.data());

    auto fShaderStr = MyGlRenderContext::getInstance()->getAssetResource("twotexture/twotexture.frag");
    LOGI("init----fShaderStr = %s", fShaderStr.data());

    m_ProgramObj = GLUtils::createProgram(vShaderStr.data(), fShaderStr.data(),
                                          m_VertexShader, m_FragmentShader);
    LOGI("m_ProgramObj = %d", m_ProgramObj);

    prepareData();
    prepareTexture();
}

void TwoTextureSample::prepareData() {
    GLfloat verticesCoords[] = {
            -0.6f,  0.6f, 0.0f,  0.0f,  0.0f,// Position 0
            -0.6f, -0.6f, 0.0f,  0.0f,  1.0f,// Position 1
            0.6f, -0.6f, 0.0f,   1.0f,  1.0f,// Position 2
            0.6f,  0.6f, 0.0f,  1.0f,  0.0f // Position 3
    };

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};
    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));

    GLuint fbo = 0;
    glGenBuffers(1, &fbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void TwoTextureSample::prepareTexture() {
    void* lyfPixel;
    int lyfWidth, lyfHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/lyf.png", &lyfPixel, lyfWidth, lyfHeiht);
    LOGI("lyfWidth = %d, lyfHeiht = %d", lyfWidth, lyfHeiht);

    void* nsPixel;
    int nsWidth, nsHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/ns.png", &nsPixel, nsWidth, nsHeiht);
    LOGI("nsWidth = %d, nsHeiht = %d", nsWidth, nsHeiht);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mLyfTextureId);
    glBindTexture(GL_TEXTURE_2D, mLyfTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexImage2D 这里出错，明天再看
    LOGI("run this line lyfPixel = %p", lyfPixel);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lyfWidth, lyfHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, lyfPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &mNsTextureId);
    glBindTexture(GL_TEXTURE_2D, mNsTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nsWidth, nsHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nsPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void TwoTextureSample::draw() {
    if (m_ProgramObj == GL_NONE) {
        return;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_ProgramObj);

    glBindVertexArray(mVaoId);

    //指定片段着色器中名为lyfId的变量，使用序号为0的纹理
    GLUtils::setUniformValue1i(m_ProgramObj, "lyfId", 0);
    //指定片段着色器中名为nsId的变量，使用序号为1的纹理
    GLUtils::setUniformValue1i(m_ProgramObj, "nsId", 1);

    //激活编号为0的纹理，此纹理使用mLyfTextureId的数据。
    //要把纹理编号指定使用什么数据，也要指定片段着色器中的变量是使用哪个编号的纹理，一个程序中可能有很多个纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mLyfTextureId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mNsTextureId);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}