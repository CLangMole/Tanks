#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

namespace RenderEngine{
    class ShaderProgram{
    public:
        ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
        ~ShaderProgram();

        ShaderProgram() = delete;
        ShaderProgram(ShaderProgram&) = delete;

        ShaderProgram &operator=(const ShaderProgram &) = delete;

        ShaderProgram(ShaderProgram &&shaderProgram) noexcept;

        ShaderProgram &operator=(ShaderProgram &&shaderProgram) noexcept;

        [[nodiscard]] bool is_compiled() const {
            return isCompiled;
        };

        void use() const;

        void set_int(const std::string &name, GLint value) const;

        void set_float(const std::string &name, GLfloat value) const;

        void set_matrix4(const std::string &name, const glm::mat4 &matrix) const;

    private:
        static bool create_shader(const std::string &shaderSource, GLenum shaderType, GLuint &shaderID);

        bool isCompiled = false;
        GLuint ID = 0;
    };
}
