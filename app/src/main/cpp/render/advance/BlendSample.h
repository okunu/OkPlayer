//
// Created by okunu on 2023/5/27.
//

#ifndef DEMO_BLENDSAMPLE_H
#define DEMO_BLENDSAMPLE_H

#include "BaseSample.h"
#include "Shader_m.h"
#include "Camera_m.h"

class BlendSample: public BaseSample {

public:
    BlendSample();
    ~BlendSample();
    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;

private:
    GLuint mVao;
    GLuint mGrassVao;
    GLuint mFirstId;
    GLuint mGrassId;
    GLuint mFloorVao;
    GLuint mFloorId;
    Shader objectShader;
    Camera camera;
};

#endif //DEMO_BLENDSAMPLE_H
