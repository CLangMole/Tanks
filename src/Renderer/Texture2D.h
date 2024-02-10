#pragma once

#include <glad/glad.h>
#include<glm/vec2.hpp>

#include <string>
#include <map>

namespace Renderer
{
    class Texture2D
    {
    public:
#pragma region
        struct SubTexture2D
        {
            glm::vec2 leftBottomUV;
            glm::vec2 rightTopUV;

            SubTexture2D(const glm::vec2 &leftBottomUV, const glm::vec2 &rightTopUV)
                    : leftBottomUV(leftBottomUV), rightTopUV(rightTopUV){}

            SubTexture2D() : leftBottomUV(0.0f), rightTopUV(1.0f){}
        };

#pragma endregion
        Texture2D(const GLuint width, const GLuint height, const unsigned char *data, const unsigned int channels = 4,
                  const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);

        Texture2D(Texture2D &&texture2D) noexcept;

        Texture2D &operator=(Texture2D &&texture2D) noexcept;

        ~Texture2D();
#pragma region

        Texture2D() = delete;

        Texture2D(const Texture2D &) = delete;

        Texture2D &operator=(const Texture2D &) = delete;

#pragma endregion
        void bind() const;

        void add_subTexture(std::string name, const glm::vec2& bottomLeftUV, const glm::vec2& rightTopUV);
        const SubTexture2D& get_subTexture(const std::string& name) const;

        const unsigned int get_width() const { return width; };
        const unsigned int get_height() const { return height; };

    private:
        GLuint ID;

        unsigned int width;
        unsigned int height;

        GLenum mode;

        std::map<std::string, SubTexture2D> subTextures;
    };
}
