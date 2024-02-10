#include "Sprite.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Renderer;

Sprite::Sprite(std::shared_ptr<Texture2D> texture2D, std::string initialSubTexture2D,
               std::shared_ptr<ShaderProgram> shader,
               const glm::vec2 &position, const glm::vec2 &scale, const float rotation)
               : texture2D(std::move(texture2D)), shader(std::move(shader)), position(position), scale(scale), rotation(rotation){

    const GLfloat vertexPosition[]{
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,

            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
    };

    Texture2D::SubTexture2D subTexture = this->texture2D->get_subTexture(std::move(initialSubTexture2D));

    const GLfloat texturePosition[]{
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.rightTopUV.y,

            subTexture.rightTopUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexPositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexPositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), &vertexPosition, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &texturePositionVBO);
    glBindBuffer(GL_ARRAY_BUFFER, texturePositionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturePosition), &texturePosition, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Sprite::~Sprite() {
    glDeleteBuffers(1, &vertexPositionVBO);
    glDeleteBuffers(1, &texturePositionVBO);
    glDeleteVertexArrays(1, &vao);
}

void Sprite::render() const {
    shader->use();
    glm::mat4 modelMatrix(1.0f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.0f));

    glBindVertexArray(vao);

    shader->set_matrix4("modelMat", modelMatrix);

    glActiveTexture(GL_TEXTURE0);
    texture2D->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
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