//
// Created by okunu on 2022/10/30.
//

#ifndef DEMO_BASESAMPLE_H
#define DEMO_BASESAMPLE_H

#include "../glUtil/GLUtil.h"

class BaseSample {

public:
    BaseSample();
    virtual ~BaseSample();
    virtual void init();
    virtual void draw();
    virtual void changeDirection(int direction);
    virtual void prepareData();
    virtual void prepareTexture();
    virtual void setAngle(float yaw, float pitch);

protected:
    GLuint m_ProgramObj;
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    void* pixel;
};

#endif //DEMO_BASESAMPLE_H
