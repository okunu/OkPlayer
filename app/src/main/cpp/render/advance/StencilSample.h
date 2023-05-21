//
// Created by okunu on 2023/5/21.
//

#ifndef DEMO_STENCILSAMPLE_H
#define DEMO_STENCILSAMPLE_H

#include "BaseSample.h"
#include "LogUtil.h"
#include "Shader_m.h"
#include "Camera_m.h"

class StencilSample: public BaseSample {

public:
    StencilSample();
    ~StencilSample();
    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;

private:
    GLuint mVao;
    GLuint mFirstId;
    GLuint mSecId;
    Shader objectShader;
    Camera camera;
};


#endif //DEMO_STENCILSAMPLE_H
