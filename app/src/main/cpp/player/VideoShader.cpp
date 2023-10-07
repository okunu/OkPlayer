//
// Created by okunu on 2023/10/1.
//

#include "VideoShader.h"

VideoShader::VideoShader() : tex_id_(0), vao_(0) {

}

VideoShader::~VideoShader() {

}

void VideoShader::init() {
    shader = Shader("texture/texture.vert", "texture/texture.frag");
    camera = Camera(glm::vec3(0.0f, 0.0f, 6.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                    -90.0f, 0.0f);
    prepareData();
    prepareTexture();
}

void VideoShader::onSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
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
    glGenTextures(1, &tex_id_);
    glBindTexture(GL_TEXTURE_2D, tex_id_);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

void VideoShader::draw(AVFrame *frame) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    glBindVertexArray(vao_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame->width, frame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame->data);
    shader.setInt("s_TextureMap", 0);

    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0);
}