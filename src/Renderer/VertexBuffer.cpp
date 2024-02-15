#include "VertexBuffer.h"

using namespace Renderer;

VertexBuffer::VertexBuffer() : index(0) {

}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &index);
}

VertexBuffer::VertexBuffer(Renderer::VertexBuffer &&vertexBuffer) noexcept {
    index = vertexBuffer.index;
    vertexBuffer.index = 0;
}

VertexBuffer &Renderer::VertexBuffer::operator=(Renderer::VertexBuffer &&vertexBuffer) noexcept {
    if (this == &vertexBuffer){
        return *this;
    }

    index = vertexBuffer.index;
    vertexBuffer.index = 0;

    return *this;
}

void VertexBuffer::init(const void *data, const unsigned int size) {
    glGenBuffers(1, &index);
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::update(const void *data, const unsigned int size) const {
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, index);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
