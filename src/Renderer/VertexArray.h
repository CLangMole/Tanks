#pragma once

#include "glad/glad.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace RenderEngine
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

#pragma region Copy and move constructors

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&& vertexArray) noexcept;
        VertexArray& operator=(VertexArray&& vertexArray) noexcept;

#pragma endregion

        void add_buffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);

        void bind() const;
        void unbind() const;
    private:
        GLuint index = 0;
        unsigned int elementsCount = 0;
    };
}
