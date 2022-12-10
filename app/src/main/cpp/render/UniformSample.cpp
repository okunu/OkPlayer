//
// Created by okunu on 2022/12/10.
//

#include "UniformSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"
#include <time.h>

UniformSample::UniformSample(): mVaoId(0) {
}

void UniformSample::init() {
    LOGI("VaoSample init");
    if (m_ProgramObj != 0) {
        return;
    }
    auto vShaderStr = MyGlRenderContext::getInstance()->getAssetResource("uniform/uniform.vert");
    LOGI("init----vShaderStr = %s", vShaderStr.data());

    auto fShaderStr = MyGlRenderContext::getInstance()->getAssetResource("uniform/uniform.frag");
    LOGI("init----fShaderStr = %s", fShaderStr.data());

    m_ProgramObj = GLUtils::createProgram(vShaderStr.data(), fShaderStr.data(),
                                          m_VertexShader, m_FragmentShader);
    LOGI("m_ProgramObj = %d", m_ProgramObj);

    prepareData();
}

void UniformSample::prepareData() {
    GLfloat vVertices[] = {
            0.0f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
    };
    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    GLuint vboId = 0;
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindVertexArray(GL_NONE);
}

void UniformSample::draw() {
    if (m_ProgramObj == 0) {
        return;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_ProgramObj);

    glBindVertexArray(mVaoId);

    time_t t = time(NULL);
    float greenValue = sin(t) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(m_ProgramObj, "outColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(GL_NONE);
    glUseProgram(GL_NONE);
}