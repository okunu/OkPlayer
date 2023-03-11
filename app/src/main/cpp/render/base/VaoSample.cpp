//
// Created by okunu on 2022/11/21.
//

#include "VaoSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0 //shader layout loc
#define VERTEX_COLOR_INDX     1 //shader layout loc

#define VERTEX_STRIDE         (sizeof(GLfloat)*(VERTEX_POS_SIZE+VERTEX_COLOR_SIZE))

VaoSample::VaoSample() {
    mVaoId = 0;
}

void VaoSample::init() {
    LOGI("VaoSample init");
    if (m_ProgramObj != 0) {
        return;
    }
    auto vShaderStr = MyGlRenderContext::getInstance()->getAssetResource("vao/vao.vert");
    LOGI("init----vShaderStr = %s", vShaderStr.data());

    auto fShaderStr = MyGlRenderContext::getInstance()->getAssetResource("vao/vao.frag");
    LOGI("init----fShaderStr = %s", fShaderStr.data());

    m_ProgramObj = GLUtils::createProgram(vShaderStr.data(), fShaderStr.data(),
                                          m_VertexShader, m_FragmentShader);
    LOGI("m_ProgramObj = %d", m_ProgramObj);

    prepareData();
}

void VaoSample::prepareData() {
    // 4 vertices, with(x,y,z) ,(r, g, b, a) per-vertex
    GLfloat vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            1.0, 0.0, 0.0,//右上角颜色

            0.5f, -0.5f, 0.0f,  // 右下角
            0.0, 0.0, 1.0,//右下角颜色

            -0.5f, -0.5f, 0.0f, // 左下角
            0.0, 1.0, 0.0,//左下角颜色

            -0.5f, 0.5f, 0.0f,   // 左上角
            0.5, 0.5, 0.5,//左上角颜色
    };

    // Index buffer data
    unsigned int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };

    glGenVertexArrays(1, &mVaoId);
    glBindVertexArray(mVaoId);

    glGenBuffers(2, mVboIds);

    glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (const void*)(3 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(GL_NONE);
}

void VaoSample::draw() {
    if (m_ProgramObj == 0) {
        return;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_ProgramObj);
    glBindVertexArray(mVaoId);
    //注意，glDrawElements函数的第三个参数，本例中为GL_UNSIGNED_INT，必须要与indices数组的类型相同，如果不同，绘制会失败
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
    glBindVertexArray(GL_NONE);
}

void VaoSample::prepareTexture() {}