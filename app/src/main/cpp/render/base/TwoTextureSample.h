//
// Created by okunu on 2022/12/10.
//

#ifndef DEMO_TWOTEXTURESAMPLE_H
#define DEMO_TWOTEXTURESAMPLE_H
#include "BaseSample.h"

class TwoTextureSample: public BaseSample {
    using SuperClass = BaseSample;

public:
    TwoTextureSample();
    void init() override;
    void draw() override;

private:
    void prepareData();
    void prepareTexture();

private:
    GLuint mVaoId;
    GLuint mLyfTextureId;
    GLuint mNsTextureId;
};



#endif //DEMO_TWOTEXTURESAMPLE_H
