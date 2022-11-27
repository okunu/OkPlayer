//
// Created by okunu on 2022/11/27.
//

#ifndef DEMO_YUVSAMPLE_H
#define DEMO_YUVSAMPLE_H

#include "BaseSample.h"

class YuvSample: public BaseSample{
    using SuperClass = BaseSample;

public:
    YuvSample();
    ~YuvSample();

    void init();
    void draw();

private:
    void prepareData();
    void prepareTexture();

private:
    GLuint mVaoId;
    GLuint yTextureId;
    GLuint uTextureId;
    GLuint vTextureId;
};


#endif //DEMO_YUVSAMPLE_H
