//
// Created by okunu on 2022/10/30.
//

#include "TextureSample.h"
#include "LogUtil.h"
#include "MyGlRenderContext.h"

TextureSample::TextureSample(): m_TextureId(0) {
}

void TextureSample::init() {
    glGenTextures(1, &m_TextureId);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, GL_NONE);

    char* vShaderStr = MyGlRenderContext::getInstance()->getAssetResource("texture/texture.vert");
    char* fShaderStr = MyGlRenderContext::getInstance()->getAssetResource("texture/texture.frag");
    m_ProgramObj = GLUtils::createProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);
    if (m_ProgramObj) {
        m_SamplerLoc = glGetUniformLocation(m_ProgramObj, "s_TextureMap");
    } else {
        LOGI("create program failed");
    }
}

void TextureSample::draw() {
    if (m_ProgramObj == GL_NONE || m_TextureId == GL_NONE) {
        return;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat verticesCoords[] = {
            -0.6f,  0.6f, 0.0f,  // Position 0
            -0.6f, -0.6f, 0.0f,  // Position 1
            0.6f, -0.6f, 0.0f,   // Position 2
            0.6f,  0.6f, 0.0f,   // Position 3
    };

    GLfloat textureCoords[] = {
            0.0f,  0.0f,        // TexCoord 0
            0.0f,  1.0f,        // TexCoord 1
            1.0f,  1.0f,        // TexCoord 2
            1.0f,  0.0f         // TexCoord 3
    };

    GLushort indices[] = {0, 1, 2, 0, 2, 3};

    glUseProgram(m_ProgramObj);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), verticesCoords);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), textureCoords);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    //如果只有一个纹理，那么这里可以不指定，系统默认会为我们赋值。
    glUniform1i(m_SamplerLoc, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

bool TextureSample::hasTextureSample() {
    return true;
}

void TextureSample::initTextureData(void* data, int width, int height) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_TextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}