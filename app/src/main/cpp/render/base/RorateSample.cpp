//
// Created by okunu on 2023/2/12.
//

#include "RorateSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"

RorateSample::RorateSample() {

}

RorateSample::~RorateSample() noexcept {

}

void RorateSample::init() {
    shader = Shader("cube/cube.vert", "cube/cube.frag");
    camera = Camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                    -90.0f, 0.0f);
    prepareData();
    prepareTexture();
}

void RorateSample::prepareData() {
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

void RorateSample::prepareTexture() {
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

void RorateSample::draw() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glBindVertexArray(mVao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFirstId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mSecId);
    shader.setInt("firId", 0);
    shader.setInt("secId", 1);


    auto rat = MyGlRenderContext::getInstance()->getWidth() * 1.0f /
               MyGlRenderContext::getInstance()->getHeight();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), rat, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RorateSample::rorate(float xoffset, float yoofset, float distance) {
    LOGI("xoofset = %f  yoofset = %f", xoffset, yoofset);
    camera.handleTouchEvent(xoffset, yoofset);
}

void RorateSample::changeDirection(int direction) {
    camera.changeDirection(direction);
}

void RorateSample::scale(float d) {

}