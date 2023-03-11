//
// Created by okunu on 2023/3/5.
//

#ifndef DEMO_PARALLELLIGHTSAMPLE_H
#define DEMO_PARALLELLIGHTSAMPLE_H

#include "BaseSample.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"
#include "Shader_m.h"
#include "Camera_m.h"

class ParallelLightSample: public BaseSample{

public:
    ParallelLightSample();
    ~ParallelLightSample();

    void init() override;
    void prepareData() override;
    void prepareTexture() override;
    void draw() override;

private:
    GLuint vao_;
    GLuint secVao_;
    GLuint textureId;
    GLuint secId;

    //光源shader
    Shader lightShader;
    //被照射物体shader
    Shader objectShader;
    Camera camera;
};

#endif //DEMO_PARALLELLIGHTSAMPLE_H
