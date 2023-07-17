//
// Created by okunu on 2023/7/17.
//

#ifndef DEMO_UNIFORMBUFFERSAMPLE_H
#define DEMO_UNIFORMBUFFERSAMPLE_H

#include "BaseSample.h"
#include "Shader_m.h"
#include "Camera_m.h"

class UniformBufferSample: public BaseSample {
public:
    UniformBufferSample();
    ~UniformBufferSample();
    void init() override;
    void draw() override;
    void prepareData() override;

private:
    GLuint mVao;
    GLuint uboMatrices;
    Shader redShader;
    Shader blueShader;
    Camera camera;
};

#endif //DEMO_UNIFORMBUFFERSAMPLE_H
