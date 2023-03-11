//
// Created by okunu on 2022/11/21.
//

#ifndef DEMO_VAOSAMPLE_H
#define DEMO_VAOSAMPLE_H

#include "BaseSample.h"

class VaoSample: public BaseSample {
    using SuperClass = BaseSample;

public:
    VaoSample();

    void init();
    void draw();

private:
    void prepareData();
    void prepareTexture() override;
private:
    GLuint mVaoId;
    GLuint mVboIds[2];
};

#endif //DEMO_VAOSAMPLE_H
