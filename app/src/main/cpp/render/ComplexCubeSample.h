//
// Created by okunu on 2023/1/7.
//

#ifndef DEMO_COMPLEXCUBESAMPLE_H
#define DEMO_COMPLEXCUBESAMPLE_H
#include "BaseSample.h"

class ComplexCubeSample: public BaseSample{
    using Super = BaseSample;
public:
    ComplexCubeSample();
    ~ComplexCubeSample();

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

#endif //DEMO_COMPLEXCUBESAMPLE_H
