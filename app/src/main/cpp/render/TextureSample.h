//
// Created by okunu on 2022/10/30.
//

#ifndef DEMO_TEXTURESAMPLE_H
#define DEMO_TEXTURESAMPLE_H
#include "BaseSample.h"

class TextureSample: public BaseSample {
    using SuperClass = BaseSample;

public:
    TextureSample();

    void init();
    void draw();
    bool hasTextureSample();
    void initTextureData(void* data, int width, int height);

private:
    GLuint m_TextureId;
    GLint m_SamplerLoc;
};

#endif //DEMO_TEXTURESAMPLE_H
