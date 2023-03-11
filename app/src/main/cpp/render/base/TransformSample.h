//
// Created by okunu on 2022/12/18.
//

#ifndef DEMO_TRANSFORMSAMPLE_H
#define DEMO_TRANSFORMSAMPLE_H

#include "BaseSample.h"

class TransformSample: public BaseSample {
    using Super = BaseSample;
public:
    TransformSample();
    ~TransformSample();

    void init() override;
    void draw() override;
    void prepareTexture() override;
    void prepareData() override;

private:
    GLuint mVaoId;
    GLuint mLyfTextureId;
    float mDegree = 0.0f;
};


#endif //DEMO_TRANSFORMSAMPLE_H
