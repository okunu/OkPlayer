//
// Created by okunu on 2022/12/10.
//

#ifndef DEMO_UNIFORMSAMPLE_H
#define DEMO_UNIFORMSAMPLE_H

#include "BaseSample.h"

class UniformSample: public BaseSample {
    using SuperClass = BaseSample;

public:
    UniformSample();
    void init() override;
    void draw() override;

    void prepareData();
    void prepareTexture() override;
private:
    GLuint mVaoId;
};

#endif //DEMO_UNIFORMSAMPLE_H
