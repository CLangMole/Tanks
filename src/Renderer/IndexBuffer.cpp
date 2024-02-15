#include "IndexBuffer.h"

using namespace Renderer;

IndexBuffer::IndexBuffer() : index(0) {

}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &index);
}

IndexBuffer::IndexBuffer(Renderer::IndexBuffer &&indexBuffer) noexcept {
    index = indexBuffer.index;
    indexBuffer.index = 0;
}

IndexBuffer &Renderer::IndexBuffer::operator=(Renderer::IndexBuffer &&indexBuffer) noexcept {
    if (this == &indexBuffer){
        return *this;
    }

    index = indexBuffer.index;
    indexBuffer.index = 0;

    return *this;
}

void IndexBuffer::init(const void *data, const unsigned int size) {
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

