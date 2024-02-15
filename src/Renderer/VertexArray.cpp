#include "VertexArray.h"

using namespace Renderer;

VertexArray::VertexArray(){
    glGenVertexArrays(1, &index);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &index);
}

VertexArray::VertexArray(Renderer::VertexArray &&vertexArray) noexcept {
    index = vertexArray.index;
    vertexArray.index = 0;
}

VertexArray &Renderer::VertexArray::operator=(Renderer::VertexArray &&vertexArray) noexcept {
    if (this == &vertexArray){
        return *this;
    }

    index = vertexArray.index;
    vertexArray.index = 0;

    return *this;
}

void VertexArray::bind() const {
    glBindVertexArray(index);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::add_buffer(const VertexBuffer &vertexBuffer, const VertexBufferLayout &layout) {
    bind();
    vertexBuffer.bind();

    const auto& layoutElements = layout.get_layoutElements();
    GLbyte* offset = nullptr;

    for (unsigned int i = 0; i < layoutElements.size(); ++i){
        const auto& currentElement = layoutElements[i];
        GLuint currentIndex = elementsCount + i;

        glEnableVertexAttribArray(currentIndex);
        glVertexAttribPointer(currentIndex, currentElement.count, currentElement.type, currentElement.normalized, static_cast<GLsizei>(layout.get_stride()), offset);
        offset += currentElement.size;
    }

    elementsCount += static_cast<unsigned int>(layoutElements.size());
}

