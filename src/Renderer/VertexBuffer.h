#pragma once
#include "glad/glad.h"

namespace Renderer
{
    class VertexBuffer
    {
    public:
        VertexBuffer();
        ~VertexBuffer();

#pragma region Copy and move constructors

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;
        VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;

#pragma endregion

        void init(const void* data, const unsigned int size);
        void update(const void* data, const unsigned int size) const;

        void bind() const;
        void unbind() const;
    private:
        GLuint index;
    };
}
