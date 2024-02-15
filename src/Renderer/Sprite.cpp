#include "Sprite.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Renderer;

Sprite::Sprite(std::shared_ptr<Texture2D> texture2D, const std::string &initialSubTexture2D,
               std::shared_ptr<ShaderProgram> shader,
               const glm::vec2 &position, const glm::vec2 &scale, const float rotation)
               : texture2D(std::move(texture2D)), shader(std::move(shader)), position(position), scale(scale), rotation(rotation){

    const GLfloat vertexPosition[]{
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };

    Texture2D::SubTexture2D subTexture = this->texture2D->get_subTexture(initialSubTexture2D);

    const GLfloat texturePosition[]{
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.leftBottomUV.y
    };

    const GLuint indexes[]{
        0, 1, 2,
        2, 3, 0
    };

    vertexPositionBuffer.init(vertexPosition, 2 * 4 * sizeof(GLfloat));

    VertexBufferLayout vertexPositionBufferLayout;
    vertexPositionBufferLayout.add_layoutElement_float(2, false);
    vertexArray.add_buffer(vertexPositionBuffer, vertexPositionBufferLayout);

    texturePositionBuffer.init(texturePosition, 2 * 4 * sizeof(GLfloat));

    VertexBufferLayout texturePositionBufferLayout;
    texturePositionBufferLayout.add_layoutElement_float(2, false);
    vertexArray.add_buffer(texturePositionBuffer, texturePositionBufferLayout);

    elementBuffer.init(indexes, 6 * sizeof(GLuint));

    vertexArray.unbind();
    elementBuffer.unbind();
}

void Sprite::render() const {
    shader->use();
    glm::mat4 modelMatrix(1.0f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.0f));

    vertexArray.bind();

    shader->set_matrix4("modelMat", modelMatrix);

    glActiveTexture(GL_TEXTURE0);
    texture2D->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    vertexArray.unbind();
}

void Sprite::set_position(const glm::vec2 &position) {
    this->position = position;
}

void Sprite::set_rotation(const float rotation) {
    this->rotation = rotation;
}

void Sprite::set_scale(const glm::vec2 &scale) {
    this->scale = scale;
}