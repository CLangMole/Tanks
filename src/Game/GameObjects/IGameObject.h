#pragma once

#include <glm/vec2.hpp>

class IGameObject
{
public:
    IGameObject(const glm::vec2 &position, const glm::vec2 &scale, float rotation);

    virtual ~IGameObject() = default;

    virtual void render() const = 0;

    virtual void update(double delta) = 0;

protected:
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
};
