#include <iostream>
#include "Texture2D.h"

using namespace RenderEngine;

Texture2D::Texture2D(const GLuint width, const GLuint height, const unsigned char *data,
                     const unsigned int channels, const GLenum filter, const GLenum wrapMode) : width(width), height(height){
    switch (channels) {
        case 4:
            mode = GL_RGBA;
            break;
        case 3:
            mode = GL_RGB;
            break;
        default:
            mode = GL_RGBA;
            break;
    }
    
    glGenTextures(1, &ID);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(Texture2D &&texture2D) noexcept {
    ID = texture2D.ID;
    mode = texture2D.mode;
    width = texture2D.width;
    height = texture2D.height;

    texture2D.ID = 0;
}

Texture2D &Texture2D::operator=(Texture2D &&texture2D) noexcept {
    if (this == &texture2D){
        return *this;
    }

    glDeleteTextures(1, &ID);

    ID = texture2D.ID;
    mode = texture2D.mode;
    width = texture2D.width;
    height = texture2D.height;

    texture2D.ID = 0;

    return *this;
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &ID);
}

void Texture2D::bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::add_subTexture(std::string name, const glm::vec2 &bottomLeftUV, const glm::vec2 &rightTopUV) {
    subTextures.emplace(name, SubTexture2D(bottomLeftUV, rightTopUV));
}

const Texture2D::SubTexture2D &Texture2D::get_subTexture(const std::string &name) const {
    auto it = subTextures.find(name);
    if (it != subTextures.end()){
        return it->second;
    }

    const static SubTexture2D defaultSubTexture;
    return defaultSubTexture;
}

