//
// Created by okunu on 2022/12/24.
//

#ifndef DEMO_CUBESAMPLE_H
#define DEMO_CUBESAMPLE_H

#include "BaseSample.h"
#include "glm-0.9.9.8/glm/glm.hpp"

class CubeSample: public BaseSample {
    using Super = BaseSample;
public:
    CubeSample();
    ~CubeSample();

    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;
    void rorate(float xoffset, float yoofset, float distance) override;
    void scale(float d) override;

private:
    GLuint mVao;
    GLuint mFirstId;
    GLuint mSecId;
    float degree;

    float xoffset;
    float yoffset;
    float distance;
    float fov;
    glm::mat4 model;
};

#endif //DEMO_CUBESAMPLE_H
