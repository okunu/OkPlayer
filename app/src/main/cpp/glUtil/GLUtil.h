//
// Created by okunu on 2022/10/29.
//

#ifndef DEMO_GLUTIL_H
#define DEMO_GLUTIL_H

#include <GLES3/gl3.h>
#include <string>

class GLUtils {
public:
    static GLuint loadShader(GLenum shaderType, const char* pSource);
    static GLuint createProgram(const char* pVertexShaderSource,
                                const char* pFragShaderSource,
                                GLuint &vertexShaderHandle,
                                GLuint &fragShaderHandle);
    static void deleteProgram(GLuint &program);
    static void checkGlError(const char* pGLOperation);
};

#endif //DEMO_GLUTIL_H
