#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <iostream>
#include <memory>

#include "Texture2D.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace RenderEngine
{
    class Sprite
    {
    public:

#pragma region FrameDescription

        struct FrameDescription
        {
            FrameDescription(const glm::vec2 leftBottomUV, glm::vec2 rightTopUV, double duration) : leftBottomUV(
                    leftBottomUV), rightTopUV(rightTopUV), duration(duration) {

            }

            glm::vec2 leftBottomUV;
            glm::vec2 rightTopUV;

            double duration;
        };

#pragma endregion

        Sprite(std::shared_ptr<Texture2D> texture2D, const std::string &initialSubTexture2D,
               std::shared_ptr<ShaderProgram> shader);

        ~Sprite() = default;

        Sprite(const Sprite &) = delete;

        Sprite &operator=(const Sprite &) = delete;

        void
        render(const glm::vec2 &position, const glm::vec2 &scale, float rotation, size_t frame = 0) const;

        void addFrames(std::vector<FrameDescription> frames);

        double get_frameDuration(size_t currentFrame) const;

        size_t get_framesCount() const;

    private:
        std::shared_ptr<Texture2D> texture2D;
        std::shared_ptr<ShaderProgram> shader;
        VertexArray vertexArray;

        VertexBuffer vertexPositionBuffer;
        VertexBuffer texturePositionBuffer;
        IndexBuffer indexBuffer;

        std::vector<FrameDescription> framesDescriptions;
        mutable size_t lastFrame;
    };
}
