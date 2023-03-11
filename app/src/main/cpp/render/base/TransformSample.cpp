//
// Created by okunu on 2022/12/18.
//

#include "TransformSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"
#include "GLUtil.h"
#include "glm-0.9.9.8/glm/vec4.hpp"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtc/matrix_transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"
#include <chrono>

using std::chrono::system_clock;

TransformSample::TransformSample() {}

TransformSample::~TransformSample() noexcept {}

void TransformSample::init() {
    if (m_ProgramObj != 0) {
        return;
    }
    auto vert = MyGlRenderContext::getInstance()->getAssetResource("transform/transform.vert");
    auto frag = MyGlRenderContext::getInstance()->getAssetResource("transform/transform.frag");
    m_ProgramObj = GLUtils::createProgram(vert.data(), frag.data(),
                                          m_VertexShader, m_FragmentShader);
    prepareData();
    prepareTexture();
}

void TransformSample::prepareData() {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)(3*sizeof(float)));

    GLuint ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void TransformSample::prepareTexture() {
    void* lyfPixel;
    int lyfWidth, lyfHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/lyf.png", &lyfPixel, lyfWidth, lyfHeiht);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mLyfTextureId);
    glBindTexture(GL_TEXTURE_2D, mLyfTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lyfWidth, lyfHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, lyfPixel);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TransformSample::draw() {
    if (m_ProgramObj == 0) {
        return;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_ProgramObj);
    glBindVertexArray(mVaoId);

    //指定片段着色器中名为lyfId的变量，使用序号为0的纹理
    GLUtils::setUniformValue1i(m_ProgramObj, "lyfId", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mLyfTextureId);

    //测试代码，位移
//    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//    glm::mat4 trans(1.0f);
//    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//    vec = trans * vec;
//    LOGI("x = %f, y = %f, z = %f", vec.x, vec.y, vec.z);

    //测试代码，旋转缩放
//    glm::mat4 trans(1.0f);
//    trans = glm::rotate(mat, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0f));
//    trans = glm::scale(mat, glm::vec3(0.8, 0.8, 0.8));

    mDegree += 1.0f;
    glm::mat4 trans(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, glm::radians((float)mDegree), glm::vec3(0.0f, 0.0f, 1.0f));
    unsigned int transformLoc = glGetUniformLocation(m_ProgramObj, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}

