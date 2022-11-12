//
// Created by okunu on 2022/10/29.
//

#include "TriangleSample.h"
#include "MyGlRenderContext.h"

TriangleSample::TriangleSample() {
}

void TriangleSample::init() {
    LOGI("TriangleSample  init");
    if (m_ProgramObj != 0) {
        return;
    }

    char* vShaderStr = MyGlRenderContext::getInstance()->getAssetResource("triangle/triangle.vert");
    char* fShaderStr = MyGlRenderContext::getInstance()->getAssetResource("triangle/triangle.frag");

    m_ProgramObj = GLUtils::createProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    LOGI("m_ProgramObj = %d", m_ProgramObj);

}

void TriangleSample::draw() {
    GLfloat vVertices[] = {
            0.0f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
    };
    if (m_ProgramObj == 0) {
        return;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_ProgramObj);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(GL_NONE);
}




