//
// Created by okunu on 2023/5/27.
//

#include <vector>
#include "BlendSample.h"
#include <map>

BlendSample::BlendSample() : mVao(0), mGrassVao(0), mFirstId(0), mGrassId(0), mFloorVao(0), mFloorId(0) {
}

BlendSample::~BlendSample() noexcept {

}

void BlendSample::init() {
    objectShader = Shader("blend/cube.vert", "blend/cube.frag");
    camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                    -90.0f, 0.0f);
    prepareData();
    prepareTexture();
}

void BlendSample::prepareData() {
    float cubeVertices[] = {
            // positions          // texture Coords
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
    GLfloat flatVertices[] = {
            5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f,  0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f,  2.0f,

            5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f,  2.0f,
            5.0f, -0.5f, -5.0f,  2.0f,  2.0f
    };
    float transparentVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
            1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

            0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
            1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
            1.0f, 0.5f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (const void *) (3 * sizeof(float)));

    glBindVertexArray(0);

    glGenVertexArrays(1, &mGrassVao);
    glBindVertexArray(mGrassVao);
    GLuint grassvbo;
    glGenBuffers(1, &grassvbo);
    glBindBuffer(GL_ARRAY_BUFFER, grassvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (const void *) (3 * sizeof(float)));
    glBindVertexArray(0);

    glGenVertexArrays(1, &mFloorVao);
    glBindVertexArray(mFloorVao);
    GLuint floorvbo;
    glGenBuffers(1, &floorvbo);
    glBindBuffer(GL_ARRAY_BUFFER, floorvbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flatVertices), flatVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (const void *) (3 * sizeof(float)));
    glBindVertexArray(0);
}

void BlendSample::prepareTexture() {
    void *firstPixel;
    int firstWidth, firstHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/lyf.png", &firstPixel, firstWidth, firstHeiht);
    LOGI("firstWidth = %d, firstHeiht = %d", firstWidth, firstHeiht);
    glGenTextures(1, &mFirstId);
    glBindTexture(GL_TEXTURE_2D, mFirstId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, firstWidth, firstHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, firstPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    void *nsPixel;
    int nsWidth, nsHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/window.png", &nsPixel, nsWidth, nsHeiht);
    LOGI("nsWidth = %d, nsHeiht = %d", nsWidth, nsHeiht);
    glGenTextures(1, &mGrassId);
    glBindTexture(GL_TEXTURE_2D, mGrassId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nsWidth, nsHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nsPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    void *floorPixel;
    int floorWidth, floorHeiht;
    MyGlRenderContext::getInstance()->getBitmap("res/floor.png", &floorPixel, floorWidth, floorHeiht);
    LOGI("floorWidth = %d, floorHeiht = %d", floorWidth, floorHeiht);
    glGenTextures(1, &mFloorId);
    glBindTexture(GL_TEXTURE_2D, mFloorId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, floorWidth, floorHeiht,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, floorPixel);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void BlendSample::draw() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::vector<glm::vec3> vegetation
            {
                    glm::vec3(-1.5f, 0.0f, -0.48f),
                    glm::vec3(1.5f, 0.0f, 0.51f),
                    glm::vec3(0.0f, 0.0f, 0.7f),
                    glm::vec3(-0.3f, 0.0f, -2.3f),
                    glm::vec3(0.5f, 0.0f, -0.6f)
            };

    objectShader.use();
    glBindVertexArray(mVao);
    auto rat = MyGlRenderContext::getInstance()->getWidth() * 1.0f /
               MyGlRenderContext::getInstance()->getHeight();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), rat, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    objectShader.setMat4("projection", projection);
    objectShader.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFirstId);
    objectShader.setInt("firId", 0);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    objectShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    objectShader.setMat4("model", model);
    glBindTexture(GL_TEXTURE_2D, mFirstId);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(mFloorVao);
    glBindTexture(GL_TEXTURE_2D, mFloorId);
    model = glm::mat4(1.0f);
    objectShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    std::map<float, glm::vec3> sorted;
    for (int i = 0; i < vegetation.size(); ++i) {
        float distance = glm::length(camera.position - vegetation[i]);
        sorted[distance] = vegetation[i];
    }

    glBindVertexArray(mGrassVao);
    glBindTexture(GL_TEXTURE_2D, mGrassId);

    for(std::map<float,glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        objectShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}