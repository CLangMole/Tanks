#pragma once

#include "ShaderProgram.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace RenderEngine
{
    class Renderer
    {
    public:
        static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shaderProgram);
        static void set_clearColor(float red, float green, float blue, float alpha);
        static void clear();
        static void set_viewport(unsigned int width, unsigned int height, unsigned int xOffset = 0, unsigned int yOffset = 0);
        static std::string get_renderer_string();
        static std::string get_version_string();
    };
}
