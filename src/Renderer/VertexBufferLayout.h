#pragma once
#include <vector>
#include "glad/glad.h"

namespace Renderer
{
    struct VertexBufferLayoutElement{
        GLint count;
        GLenum type;
        GLboolean normalized;
        unsigned int size;
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout();

        void reserve_elements(const size_t count);

        unsigned int get_stride() const {
            return stride;
        };

        void add_layoutElement_float(const int count, const bool normalized);
        const std::vector<VertexBufferLayoutElement>& get_layoutElements() const {
            return layoutElements;
        };
    private:
        std::vector<VertexBufferLayoutElement> layoutElements;
        unsigned int stride;
    };
}
