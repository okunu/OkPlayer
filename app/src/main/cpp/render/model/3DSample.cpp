//
// Created by okunu on 2023/4/5.
//

#include "3DSample.h"

Type3DSample::Type3DSample(): xoffset(0.0f), yoffset(0.0f), distance(0.0f) {
    glm::mat4 tmpMat = glm::mat4(1.0f);
    model = glm::translate(tmpMat, glm::vec3(0.0f, -3.0f, 0.0f));
}

Type3DSample::~Type3DSample() {}

void Type3DSample::init() {
    shader = Shader("3D/3d.vert", "3D/3d.frag");
    camera = Camera(glm::vec3(0.0f, 0.0f, 13.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                    -90.0f, 0.0f);
    ourModel = Model("/data/data/com.ou.demo/files/model/poly/Apricot_02_hi_poly.obj");
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


    if (distance != 0) {
        glm::vec3 cross = glm::cross(glm::vec3(0.0f, 0.0f, 1.0), glm::vec3(xoffset, yoffset, 0.0f));
        glm::mat4 tmpMat = glm::mat4(1.0f);
        model = glm::rotate(model, distance, cross);
        tmpMat *= model;
        model = tmpMat;
    }

    shader.setMat4("model", model);

    ourModel.Draw(shader);
    distance = 0.0f;
}

void Type3DSample::rorate(float x, float y, float distance) {
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