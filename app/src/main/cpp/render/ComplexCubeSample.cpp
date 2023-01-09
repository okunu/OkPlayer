//
// Created by okunu on 2023/1/7.
//

#include "ComplexCubeSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"
#include "CommonData.h"

ComplexCubeSample::ComplexCubeSample(): mVao(0), mFirstId(0), mSecId(0), degree(0.0f) {
    cameraPos   = glm::vec3(0.0f, 0.0f,  13.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
}

ComplexCubeSample::~ComplexCubeSample() noexcept {}

void ComplexCubeSample::init() {
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

void ComplexCubeSample::prepareData() {
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

void ComplexCubeSample::prepareTexture() {
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

void ComplexCubeSample::draw() {
    if (m_ProgramObj == 0) {
        return;
    }
    //注释内容放开并修改model矩阵，就可以看到10个矩阵了
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

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

    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);

    degree = degree + 1.0f;

    float radius = 8.0f;
    float temp = degree * 0.01;
    float camX = sin(temp) * radius;
    float camZ = cos(temp) * radius;
//    cameraPos = glm::vec3(camX, 0.0, camZ);
    view = glm::lookAt(cameraPos,
                       cameraPos + cameraFront,
                       cameraUp);
    auto rat = MyGlRenderContext::getInstance()->getWidth() * 1.0f / MyGlRenderContext::getInstance()->getHeight();
    projection = glm::perspective(glm::radians(45.0f), rat, 0.1f, 100.0f);

    auto modelLoc = glGetUniformLocation(m_ProgramObj, "model");
    auto viewLoc = glGetUniformLocation(m_ProgramObj, "view");
    auto projectionLoc = glGetUniformLocation(m_ProgramObj, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    for (int i = 0; i < 10; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void ComplexCubeSample::changeDirection(int direction) {
    LOGI("direction = %d", direction);
    float cameraSpeed = 0.5f;
    switch (direction) {
        case Direction::Up:
            cameraPos += cameraSpeed * cameraFront;
            break;
        case Direction::Down:
            cameraPos -= cameraSpeed * cameraFront;
            break;
        case Direction::Left:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
            break;
        case Direction::Right:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp) * cameraSpeed);
            break;
    }
    LOGI("cameraPos.x = %f, cameraPos.y = %f, cameraPos.z = %f", cameraPos.x, cameraPos.y, cameraPos.z);
    auto temp = cameraPos + cameraFront;
    LOGI("cameraFront.x = %f, cameraFront.y = %f, cameraFront.z = %f", cameraFront.x, cameraFront.y, cameraFront.z);
    LOGI("temp.x = %f, temp.y = %f, temp.z = %f", temp.x, temp.y, temp.z);
}

void ComplexCubeSample::setAngle(float yaw, float pitch) {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    LOGI("cameraFront.x = %f, cameraFront.y = %f, cameraFront.z = %f", cameraFront.x, cameraFront.y, cameraFront.z);
    LOGI("cameraPos.x = %f, cameraPos.y = %f, cameraPos.z = %f", cameraPos.x, cameraPos.y, cameraPos.z);
//    auto temp = cameraPos + cameraFront;
//    LOGI("temp.x = %f, temp.y = %f, temp.z = %f", temp.x, temp.y, temp.z);
}