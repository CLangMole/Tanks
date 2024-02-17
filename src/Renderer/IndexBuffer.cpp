#include "IndexBuffer.h"

using namespace RenderEngine;

IndexBuffer::IndexBuffer() : index(0), count(0) {

}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &index);
}

IndexBuffer::IndexBuffer(RenderEngine::IndexBuffer &&indexBuffer) noexcept {
    index = indexBuffer.index;
    indexBuffer.index = 0;

    count = indexBuffer.count;
    indexBuffer.count = 0;
}

IndexBuffer &RenderEngine::IndexBuffer::operator=(RenderEngine::IndexBuffer &&indexBuffer) noexcept {
    if (this == &indexBuffer){
        return *this;
    }

    index = indexBuffer.index;
    indexBuffer.index = 0;

    count = indexBuffer.count;
    indexBuffer.count = 0;

    return *this;
}

void IndexBuffer::init(const void *data, const unsigned int count) {
    this->count = count;

    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

