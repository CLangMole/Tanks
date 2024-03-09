#pragma once

#include <glm/vec2.hpp>
#include <vector>
#include "../../Physics/PhysicsEngine.h"

class IGameObject
{
public:
    IGameObject(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer);

    virtual ~IGameObject() = default;

    virtual void render() const = 0;

    virtual void update(double delta) {};

    virtual glm::vec2 &get_currentPosition() {
        return position;
    }

    virtual glm::vec2 &get_currentDirection() {
        return direction;
    }

    virtual double get_currentVelocity() {
        return currentVelocity;
    }

    virtual void set_velocity(double newVelocity);

    [[nodiscard]] const std::vector<Physics::AABB> &get_colliders() const {
        return colliders;
    }

    [[nodiscard]] const glm::vec2 &get_scale() const {
        return scale;
    }

protected:
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;
    float layer;

    glm::vec2 direction;
    double currentVelocity;
    std::vector<Physics::AABB> colliders;
};
