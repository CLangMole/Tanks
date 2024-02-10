#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"

namespace Renderer{
    ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader){
        GLuint vertexShaderID;
        if(!create_shader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)){
            std::cerr << "VERTEX_SHADER: Compile-time error" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }

        GLuint fragmentShaderID;
        if (!create_shader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)){
            std::cerr << "FRAGMENT_SHADER: Compile-time error" << std::endl;
            glDeleteShader(fragmentShaderID);
            return;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertexShaderID);
        glAttachShader(ID, fragmentShaderID);
        glLinkProgram(ID);

        GLint success;
        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success){
            GLchar infoLog[1024];
            glGetShaderInfoLog(ID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
        }
        else{
            isCompiled = true;
        }

        glDeleteShader(fragmentShaderID);
        glDeleteShader(vertexShaderID);
    }

    bool ShaderProgram::create_shader(const std::string &shaderSource, const GLenum shaderType, GLuint &shaderID) {
        shaderID = glCreateShader(shaderType);
        const char* code = shaderSource.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (!success){
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
            return false;
        }

        return true;
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(ID);
    }

    void ShaderProgram::use() const {
        glUseProgram(ID);
    }

    ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram) noexcept {
        ID = shaderProgram.ID;
        isCompiled = shaderProgram.isCompiled;

        shaderProgram.ID = 0;
        shaderProgram.isCompiled = false;
    }

    ShaderProgram &ShaderProgram::operator=(ShaderProgram &&shaderProgram) noexcept {
        if (this == &shaderProgram){
            return *this;
        }

        glDeleteProgram(ID);
        ID = shaderProgram.ID;
        isCompiled = shaderProgram.isCompiled;

        shaderProgram.ID = 0;
        shaderProgram.isCompiled = false;

        return *this;
    }

    void ShaderProgram::set_int(const std::string &name, const GLint value) {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void ShaderProgram::set_matrix4(const std::string &name, const glm::mat4& matrix) {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

