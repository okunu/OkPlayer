//
// Created by okunu on 2023/4/5.
//

#include "3DSample.h"

Type3DSample::Type3DSample() {
}

Type3DSample::~Type3DSample() {}

void Type3DSample::init() {
    shader = Shader("3D/3d.vert", "3D/3d.frag");
    camera = Camera(glm::vec3(0.0f, 0.0f, 13.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                    -90.0f, 0.0f);
    ourModel = Model("/data/data/com.ou.demo/files/model/nanosuit");
}

void Type3DSample::draw() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    shader.use();
    auto rat = MyGlRenderContext::getInstance()->getWidth() * 1.0f /
               MyGlRenderContext::getInstance()->getHeight();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), rat, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setMat4("model", model);

    ourModel.Draw(shader);
}