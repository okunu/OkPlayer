//
// Created by okunu on 2023/10/1.
//

#include "VideoShader.h"

VideoShader::VideoShader() : texture{0}, vao_(0) {

}

VideoShader::~VideoShader() {

}

void VideoShader::init() {
    shader = Shader("video/video.vert", "video/video.frag");
//    camera = Camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f),
//                    -90.0f, 0.0f);
    prepareData();
}

void VideoShader::onSurfaceChanged(int width, int height, int sw, int sh) {
    screen_width_ = width;
    screen_height_ = height;
    scale_width = sw;
    scale_height = sh;
    glViewport(0, 0, screen_width_, screen_height_);
    prepareTexture();
}

void VideoShader::prepareData() {
    GLfloat verticesCoords[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Position 0
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,// Position 1
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // Position 2
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f  // Position 3
    };

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    unsigned int vboIds[2];
    glGenBuffers(2, vboIds);

    //绑定vbo[0]，GL_ARRAY_BUFFER类型的buffer，它只能有一个，所有三角形顶点和纹理就放在一起解析了
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCoords), verticesCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5* sizeof(float), (const void*)(3 * sizeof(float)));
    //绑定vbo[1]，GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(GL_NONE);

}

void VideoShader::prepareTexture() {
    glGenTextures(3, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, scale_width, scale_height, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, scale_width/2, scale_height/2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, scale_width/2, scale_height/2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, NULL);
}

void VideoShader::draw(uint8_t* data[]) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    yuv_data[0] = data[0];
    yuv_data[1] = data[1];
    yuv_data[2] = data[2];

    shader.use();
    glBindVertexArray(vao_);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, scale_width, scale_height, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, yuv_data[0]);
    shader.setInt("yTexture", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, scale_width/2, scale_height/2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, yuv_data[1]);
    shader.setInt("uTexture", 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, scale_width/2, scale_height/2, 0, GL_LUMINANCE,
                 GL_UNSIGNED_BYTE, yuv_data[2]);
    shader.setInt("vTexture", 2);

//    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
//    glm::mat4 view = camera.getViewMatrix();
//    glm::mat4 model = glm::mat4(1.0f);
//    shader.setMat4("projection", projection);
//    shader.setMat4("view", view);
//    shader.setMat4("model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}