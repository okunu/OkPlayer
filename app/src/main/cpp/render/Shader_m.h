//
// Created by okunu on 2023/2/4.
//

#ifndef DEMO_SHADER_M_H
#define DEMO_SHADER_M_H

#include "MyGlRenderContext.h"
#include <string>
#include "glm-0.9.9.8/glm/glm.hpp"
#include "glm-0.9.9.8/glm/gtx/transform.hpp"
#include "glm-0.9.9.8/glm/gtc/type_ptr.hpp"

const static std::string PROGRAM = "PROGRAM";

class Shader{
public:
    unsigned int ID;

    Shader(){};

    Shader(const char* vertexPath, const char* fragmentPath){
        auto vertexCode = MyGlRenderContext::getInstance()->getAssetResource(vertexPath);
        LOGI("path = %s", vertexPath);
        auto fragmentCode = MyGlRenderContext::getInstance()->getAssetResource(fragmentPath);
        unsigned int vertex, fragment;
        auto realVertextCode = vertexCode.data();
        auto realFrgmentCode = fragmentCode.data();
        //LOGI("realVertextCode = %s, realFrgmentCode = %s", realVertextCode, realFrgmentCode);
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &realVertextCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEXT");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &realFrgmentCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, PROGRAM);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const{
        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec2(const std::string& name, const glm::vec2& value) {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string& name, float x, float y) {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    void setVec3(const std::string& name, const glm::vec3& value) {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string& name, float x, float y, float z) {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string& name, const glm::vec4& value) {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string& name, float x, float y, float z, float w) {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string& name, const glm::mat2& mat) {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string& name, const glm::mat3& mat) {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompileErrors(GLuint shader, std::string type) {
        GLint success;
        GLchar infoLog[1024];
        if(type != PROGRAM) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                LOGI("shader compile error type = %s, infoLog = ",type.data(), infoLog);
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                LOGI("program link error  type = %s, infoLog = ",type.data(), infoLog);
            }
        }
    }
};

#endif //DEMO_SHADER_M_H
