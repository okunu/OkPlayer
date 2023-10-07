//
// Created by okunu on 2023/10/1.
//

#ifndef OKPLAYER_VIDEOSHADER_H
#define OKPLAYER_VIDEOSHADER_H

#include "GLUtil.h"
#include "Shader_m.h"
#include "Camera_m.h"

extern "C" {
#include "libavcodec/avcodec.h"
}

class VideoShader {

public:
    VideoShader();
    ~VideoShader();

    void init();
    void onSurfaceChanged(int width, int height);
    void draw(AVFrame* frame);

private:
    void prepareData();
    void prepareTexture();

private:
    Shader shader;
    Camera camera;
    GLuint tex_id_;
    GLuint vao_;
};

#endif //OKPLAYER_VIDEOSHADER_H
