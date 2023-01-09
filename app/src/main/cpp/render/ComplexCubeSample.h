//
// Created by okunu on 2023/1/7.
//

#ifndef DEMO_COMPLEXCUBESAMPLE_H
#define DEMO_COMPLEXCUBESAMPLE_H
#include "BaseSample.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"

class ComplexCubeSample: public BaseSample{
    using Super = BaseSample;
public:
    ComplexCubeSample();
    ~ComplexCubeSample();

    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;
    void changeDirection(int direction) override;
    void setAngle(float yaw, float pitch) override;

private:
    GLuint mVao;
    GLuint mFirstId;
    GLuint mSecId;
    float degree;
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
};

#endif //DEMO_COMPLEXCUBESAMPLE_H
