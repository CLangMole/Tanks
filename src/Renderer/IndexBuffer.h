#pragma once
#include "glad/glad.h"

namespace RenderEngine{
    class IndexBuffer
    {
    public:
        IndexBuffer();
        ~IndexBuffer();

#pragma region Copy and move constructors

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&& indexBuffer) noexcept;
        IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;

#pragma endregion
        void init(const void* data, const unsigned int count);

        void bind() const;
        void unbind() const;

        unsigned int get_count() const{
            return count;
        }
    private:
        GLuint index;
        unsigned int count;
    };
}
