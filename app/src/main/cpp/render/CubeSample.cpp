//
// Created by okunu on 2022/12/24.
//

#include "CubeSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"

CubeSample::CubeSample(): mVao(0), mFirstId(0), mSecId(0), degree(0.0f),
    xoffset(0.0f), yoffset(0.0f), distance(0.0f), fov(45.0f){
    model = glm::mat4(1.0f);
}

CubeSample::~CubeSample() noexcept {}

void CubeSample::init() {
    if (m_ProgramObj != 0) {
        return;
    }
    auto vert = MyGlRenderContext::getInstance()->getAssetResource("cube/cube.vert");
    auto frag = MyGlRenderContext::getInstance()->getAssetResource("cube/cube.frag");
    m_ProgramObj = GLUtils::createProgram(vert.data(), frag.data(),
                                          m_VertexShader, m_FragmentShader);
    prepareData();
    prepareTexture();
}

void CubeSample::prepareData() {
    float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)(3*sizeof(float)));

    glBindVertexArray(0);
}

void CubeSample::prepareTexture() {
    void* firstPixel;
    int firstWidth, firstHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/wall.png", &firstPixel, firstWidth, firstHeiht);
    LOGI("firstWidth = %d, firstHeiht = %d", firstWidth, firstHeiht);

    void* secPixel;
    int secWidth, secHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/face.png", &secPixel, secWidth, secHeiht);
    LOGI("secWidth = %d, secHeiht = %d", secWidth, secHeiht);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mFirstId);
    glBindTexture(GL_TEXTURE_2D, mFirstId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, firstWidth, firstHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, firstPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &mSecId);
    glBindTexture(GL_TEXTURE_2D, mSecId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, secWidth, secHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, secPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void CubeSample::draw() {
    if (m_ProgramObj == 0) {
        return;
    }
    //注释内容放开并修改model矩阵，就可以看到10个矩阵了
//    glm::vec3 cubePositions[] = {
//            glm::vec3( 0.0f,  0.0f,  0.0f),
//            glm::vec3( 2.0f,  5.0f, -15.0f),
//            glm::vec3(-1.5f, -2.2f, -2.5f),
//            glm::vec3(-3.8f, -2.0f, -12.3f),
//            glm::vec3( 2.4f, -0.4f, -3.5f),
//            glm::vec3(-1.7f,  3.0f, -7.5f),
//            glm::vec3( 1.3f, -2.0f, -2.5f),
//            glm::vec3( 1.5f,  2.0f, -2.5f),
//            glm::vec3( 1.5f,  0.2f, -1.5f),
//            glm::vec3(-1.3f,  1.0f, -1.5f)
//    };

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);
    glBindVertexArray(mVao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFirstId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mSecId);

    GLUtils::setUniformValue1i(m_ProgramObj, "firId", 0);
    GLUtils::setUniformValue1i(m_ProgramObj, "secId", 1);

    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    degree = degree + 1.0f;

    model = glm::rotate(model, glm::radians(degree), glm::vec3(0.5f, 1.0f, 0.0f));

    if (distance != 0) {
        glm::vec3 cross = glm::cross(glm::vec3(0.0f, 0.0f, 1.0), glm::vec3(xoffset, yoffset, 0.0f));
        glm::mat4 tmpMat = glm::mat4(1.0f);
        model = glm::rotate(model, distance, cross);
        tmpMat *= model;
        model = tmpMat;
    }

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    auto rat = MyGlRenderContext::getInstance()->getWidth() * 1.0f / MyGlRenderContext::getInstance()->getHeight();
//    projection = glm::perspective(glm::radians(45.0f), rat, 0.1f, 100.0f);
    projection = glm::perspective(glm::radians(fov), rat, 0.1f, 100.0f);

    auto modelLoc = glGetUniformLocation(m_ProgramObj, "model");
    auto viewLoc = glGetUniformLocation(m_ProgramObj, "view");
    auto projectionLoc = glGetUniformLocation(m_ProgramObj, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

//    for (int i = 0; i < 10; ++i) {
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, cubePositions[i]);
//        model = glm::rotate(model, glm::radians(degree), glm::vec3(1.0f, 0.3f, 0.5f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    distance = 0.0f;
}

void CubeSample::rorate(float x, float y, float distance) {
    distance = distance/80.0f;
    if (x > 0 && y > 0) {
        y = -y;
    } else if (x <= 0 && y > 0) {
        x = -x;
        distance = -distance;
    } else if (x > 0 && y < 0) {
        y = -y;
    } else if (x <= 0 && y < 0) {
        y = -y;
    }
    this->xoffset = x;
    this->yoffset = y;
    this->distance = distance;
}

void CubeSample::scale(float scale) {
    if (scale > 1) {
        fov -= scale;
    } else if(scale<1) {
        fov +=  (1.0f/scale);
    }
    if (fov <= 20.0) {
        fov = 20.0;
    } else if (fov > 140.0) {
        fov = 140.0;
    }
}