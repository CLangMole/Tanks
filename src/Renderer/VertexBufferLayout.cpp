#include "VertexBufferLayout.h"

using namespace Renderer;

VertexBufferLayout::VertexBufferLayout() : stride(0){

}

void VertexBufferLayout::reserve_elements(const size_t count) {
    layoutElements.reserve(count);
}

void VertexBufferLayout::add_layoutElement_float(const int count, const bool normalized) {
    layoutElements.push_back(VertexBufferLayoutElement({ count, GL_FLOAT, normalized, count * static_cast<unsigned int>(sizeof(GLfloat)) }));
    stride += layoutElements.back().size;
}
