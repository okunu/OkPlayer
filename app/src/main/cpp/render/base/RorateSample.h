//
// Created by okunu on 2023/2/12.
//

#ifndef DEMO_RORATESAMPLE_H
#define DEMO_RORATESAMPLE_H

#include "BaseSample.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"
#include "Shader_m.h"
#include "Camera_m.h"

class RorateSample: public BaseSample {

    using Super = BaseSample;
public:
    RorateSample();
    ~RorateSample();

    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;
    void rorate(float xoffset, float yoofset, float distance) override;
    void scale(float d) override;
    void changeDirection(int direction) override;

private:
    Shader shader;
    Camera camera;
    GLuint mVao;
    GLuint mFirstId;
    GLuint mSecId;

    float fov;
    glm::mat4 model;
};

#endif //DEMO_RORATESAMPLE_H
