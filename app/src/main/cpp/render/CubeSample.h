//
// Created by okunu on 2022/12/24.
//

#ifndef DEMO_CUBESAMPLE_H
#define DEMO_CUBESAMPLE_H

#include "BaseSample.h"

class CubeSample: public BaseSample {
    using Super = BaseSample;
public:
    CubeSample();
    ~CubeSample();

    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;

private:
    GLuint mVao;
    GLuint mFirstId;
    GLuint mSecId;
    float degree;
};

#endif //DEMO_CUBESAMPLE_H
