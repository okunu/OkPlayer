//
// Created by okunu on 2022/10/30.
//

#include "TextureSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"
#include <thread>

TextureSample::TextureSample(): m_TextureId(0) {
}

void TextureSample::init() {
    auto vShaderStr = MyGlRenderContext::getInstance()->getAssetResource("texture/texture.vert");
    auto fShaderStr = MyGlRenderContext::getInstance()->getAssetResource("texture/texture.frag");
    m_ProgramObj = GLUtils::createProgram(vShaderStr.data(), fShaderStr.data(),
                                          m_VertexShader, m_FragmentShader);
    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    } else {
        LOGI("create program failed");
    }
    prepareData();
    prepareTexture();
}

void TextureSample::prepareData() {
    //这里包含了三角形顶点坐标，也包含了纹理顶点坐标，和解析颜色一样解析
    GLfloat verticesCoords[] = {
            -0.6f,  0.6f, 0.0f,  0.0f,  0.0f,// Position 0
            -0.6f, -0.6f, 0.0f,  0.0f,  1.0f,// Position 1
            0.6f, -0.6f, 0.0f,   1.0f,  1.0f,// Position 2
            0.6f,  0.6f, 0.0f,  1.0f,  0.0f // Position 3
    };

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    //生成vao并绑定vao
    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    unsigned int vboIds[2];
    glGenBuffers(2, vboIds);

    //绑定vbo[0]，GL_ARRAY_BUFFER类型的buffer，它只能有一个，所有三角形顶点和纹理就放在一起解析了
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)(3 * sizeof(float)));
    //绑定vbo[1]，GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(GL_NONE);
}

void TextureSample::draw() {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_ProgramObj);

    glBindVertexArray(mVaoId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    //如果只有一个纹理，那么这里可以不指定，系统默认会为我们赋值。
    glUniform1i(m_SamplerLoc, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}

void TextureSample::prepareTexture() {
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    void* lyfPixel = nullptr;
    int lyfWidth, lyfHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/ns.png", &lyfPixel, lyfWidth, lyfHeiht);
    LOGI("lyfWidth = %d, lyfHeiht = %d, threadid = %d", lyfWidth, lyfHeiht, std::this_thread::get_id());
    if (lyfPixel == nullptr) {
        LOGI("pixel is nullptr");
    } else {
        LOGI("pixel is not nullptr");
    }
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lyfWidth, lyfHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, lyfPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}