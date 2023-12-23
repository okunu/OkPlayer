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
    void onSurfaceChanged(int width, int height, int scale_width, int scale_height);
    void draw(uint8_t* data[]);

private:
    void prepareData();
    void prepareTexture();

private:
    Shader shader;
    Camera camera;
    GLuint texture[3];
    GLuint vao_;
    int width_;
    int height_;

    uint8_t *yuv_data[3];
    int scale_width;
    int scale_height;
};

#endif //OKPLAYER_VIDEOSHADER_H
