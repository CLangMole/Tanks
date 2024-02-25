#include "Sprite.h"
#include "Renderer.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

using namespace RenderEngine;

Sprite::Sprite(std::shared_ptr<Texture2D> texture2D, const std::string &initialSubTexture2D,
               std::shared_ptr<ShaderProgram> shader)
        : texture2D(std::move(texture2D)), shader(std::move(shader)), lastFrame(0) {

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

    indexBuffer.init(indexes, 6);

    vertexArray.unbind();
    indexBuffer.unbind();
}

void Sprite::render(const glm::vec2 &position, const glm::vec2 &scale, const float rotation, const size_t frame) const {
    if (lastFrame != frame) {
        lastFrame = frame;

        const FrameDescription &currentFrameDescription = framesDescriptions[frame];

        const GLfloat texturePosition[]{
                currentFrameDescription.leftBottomUV.x, currentFrameDescription.leftBottomUV.y,
                currentFrameDescription.leftBottomUV.x, currentFrameDescription.rightTopUV.y,
                currentFrameDescription.rightTopUV.x, currentFrameDescription.rightTopUV.y,
                currentFrameDescription.rightTopUV.x, currentFrameDescription.leftBottomUV.y
        };

        texturePositionBuffer.update(texturePosition, 2 * 4 * sizeof(GLfloat));
    }

    shader->use();
    glm::mat4 modelMatrix(1.0f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0.0f));

    shader->set_matrix4("modelMat", modelMatrix);

    glActiveTexture(GL_TEXTURE0);
    texture2D->bind();

    Renderer::draw(vertexArray, indexBuffer, *shader);
}

void Sprite::addFrames(std::vector<FrameDescription> frames) {
    this->framesDescriptions = std::move(frames);
}

double Sprite::get_frameDuration(const size_t currentFrame) const {
    return framesDescriptions[currentFrame].duration;
}

size_t Sprite::get_framesCount() const {
    return framesDescriptions.size();
}
