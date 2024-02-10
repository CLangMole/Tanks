#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <memory>

#include "Texture2D.h"
#include "ShaderProgram.h"

namespace Renderer
{
    class Sprite
    {
    public:
        Sprite(std::shared_ptr<Texture2D> texture2D, std::string initialSubTexture2D,
               std::shared_ptr<ShaderProgram> shader,
               const glm::vec2 &position = glm::vec2(0.0f), const glm::vec2 &scale = glm::vec2(1.0f),
               const float rotation = 0.0f);

        ~Sprite();

        Sprite(const Sprite&) = delete;
        Sprite& operator=(const Sprite&) = delete;

        virtual void render() const;

        void set_position(const glm::vec2& position);
        void set_scale(const glm::vec2& scale);
        void set_rotation(const float rotation);
    protected:
        std::shared_ptr<Texture2D> texture2D;
        std::shared_ptr<ShaderProgram> shader;

        glm::vec2 position;
        glm::vec2 scale;
        float rotation;

        GLuint vao;
        GLuint vertexPositionVBO;
        GLuint texturePositionVBO;
    };
}
