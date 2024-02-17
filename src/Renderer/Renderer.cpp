#include "Renderer.h"

using namespace RenderEngine;

void Renderer::draw(const RenderEngine::VertexArray &vertexArray, const RenderEngine::IndexBuffer &indexBuffer,
                             const RenderEngine::ShaderProgram &shaderProgram) {
    shaderProgram.use();
    vertexArray.bind();
    indexBuffer.bind();

    glDrawElements(GL_TRIANGLES, indexBuffer.get_count(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::set_clearColor(float red, float green, float blue, float alpha) {
    glClearColor(red, green, blue, alpha);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::set_viewport(unsigned int height, unsigned int width, unsigned int xOffset, unsigned int yOffset) {
    glViewport(xOffset, yOffset, width, height);
}

std::string Renderer::get_renderer_string() {
    return (char*)glGetString(GL_RENDERER);
}

std::string Renderer::get_version_string() {
    return (char*)glGetString(GL_VERSION);
}
