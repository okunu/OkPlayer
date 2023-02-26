//
// Created by okunu on 2023/1/25.
//

#ifndef DEMO_MATERIALSAMPLE_H
#define DEMO_MATERIALSAMPLE_H

#include "BaseSample.h"
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"
#include "Shader_m.h"
#include "Camera_m.h"

class MaterialSample: public BaseSample{
    using Super = BaseSample;
public:
    MaterialSample();
    ~MaterialSample();

    void init() override;
    void draw() override;
    void prepareData() override;

private:
    GLuint vao_;
    GLuint secVao_;

    //光源shader
    Shader lightShader;
    //被照射物体shader
    Shader objectShader;
    Camera camera;
};

#endif //DEMO_MATERIALSAMPLE_H
