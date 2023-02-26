//
// Created by okunu on 2023/2/26.
//

#ifndef DEMO_LIGHTSAMPLE_H
#define DEMO_LIGHTSAMPLE_H
#include "BaseSample.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"
#include "Shader_m.h"
#include "Camera_m.h"

class LightSample: public BaseSample{
    using Super = BaseSample;
public:
    LightSample();
    ~LightSample();

    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;

private:
    GLuint vao_;
    GLuint secVao_;
    GLuint textureId;

    //光源shader
    Shader lightShader;
    //被照射物体shader
    Shader objectShader;
    Camera camera;
};
#endif //DEMO_LIGHTSAMPLE_H
