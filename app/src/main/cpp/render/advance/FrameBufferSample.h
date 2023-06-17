//
// Created by okunu on 2023/6/17.
//

#ifndef DEMO_FRAMEBUFFERSAMPLE_H
#define DEMO_FRAMEBUFFERSAMPLE_H

#include "BaseSample.h"
#include "Shader_m.h"
#include "Camera_m.h"

class FrameBufferSample: public BaseSample {
public:
    FrameBufferSample();
    ~FrameBufferSample();
    void init() override;
    void draw() override;
    void prepareData() override;
    void prepareTexture() override;
    void onSurfaceChanged() override;

private:
    GLuint mCubeVao;
    GLuint mFloorVao;
    GLuint mScreenVao;
    GLuint mCubeId;
    GLuint mFloorId;
    GLuint mFrameBufferId;
    GLuint mFrameTextureId;
    Shader frameShader;
    Shader screenShader;
    Camera camera;
};

#endif //DEMO_FRAMEBUFFERSAMPLE_H
