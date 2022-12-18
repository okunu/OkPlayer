//
// Created by okunu on 2022/10/29.
//

#include "GLUtil.h"
#include "LogUtil.h"
#include <chrono>

GLuint GLUtils::loadShader(GLenum shaderType, const char *pSource) {
    LOGI("create shader = %d  source = %s", shaderType, pSource);
    GLuint shader = glCreateShader(shaderType);
    LOGI("create shader = %d", shader);
    if (shader) {
        glShaderSource(shader, 1, &pSource, nullptr);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = new char[infoLen];
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, nullptr, buf);
                    LOGI("compile shader failed, type = %d, info = %s", shaderType, buf);
                    delete[] buf;
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }
}

GLuint GLUtils::createProgram(const char *pVertexShaderSource, const char *pFragShaderSource,
                              GLuint &vertexShaderHandle, GLuint &fragShaderHandle) {
    GLuint program = 0;
    vertexShaderHandle = loadShader(GL_VERTEX_SHADER, pVertexShaderSource);
    if (!vertexShaderHandle) return program;
    fragShaderHandle = loadShader(GL_FRAGMENT_SHADER, pFragShaderSource);
    if (!fragShaderHandle) return program;

    program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShaderHandle);
        checkGlError("glAttachShader");
        glAttachShader(program, fragShaderHandle);
        checkGlError("glAttachShader");

        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        glDetachShader(program, vertexShaderHandle);
        glDeleteShader(vertexShaderHandle);
        vertexShaderHandle = 0;
        glDetachShader(program, fragShaderHandle);
        glDeleteShader(fragShaderHandle);
        fragShaderHandle = 0;

        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = new char[bufLength];
                LOGI("link program failed info = %s", buf);
                delete[] buf;
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    LOGI("create program = %d", program);
    return program;
}

void GLUtils::deleteProgram(GLuint &program) {
    LOGI("delete program");
    if (program) {
        glUseProgram(0);
        glDeleteProgram(program);
        program = 0;
    }
}

void GLUtils::checkGlError(const char *pGLOperation) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGI("check error operation is %s, glerror is %s", pGLOperation, error);
    }
}

void GLUtils::setUniformValue1i(GLuint program, const char *name, GLint value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

std::time_t GLUtils::getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
    //std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);
    return timestamp;
}
