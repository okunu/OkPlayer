//
// Created by okunu on 2023/6/23.
//

#ifndef DEMO_SKYBOXSAMPLE_H
#define DEMO_SKYBOXSAMPLE_H

#include "BaseSample.h"
#include "Shader_m.h"
#include "Camera_m.h"

class SkyBoxSample: public BaseSample {
public:
    SkyBoxSample();
    ~SkyBoxSample();
    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;

private:
    GLuint mSkyVao;
    GLuint mSkyId;
    GLuint mCubeVao;
    GLuint mCubeId;
    Shader shader;
    Shader cubeShader;
    Camera camera;
};

#endif //DEMO_SKYBOXSAMPLE_H
