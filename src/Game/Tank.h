#pragma once

#include <iostream>
#include <memory>
#include <glm/vec2.hpp>

#include "..//Renderer/AnimatedSprite.h"

class Tank
{
public:
#pragma region
    enum class Rotation{
        Top,
        Bottom,
        Left,
        Right
    };
#pragma endregion
    Tank(std::shared_ptr<RenderEngine::AnimatedSprite> sprite, const float velocity, const glm::vec2& position);

    void render() const;
    void update(const uint64_t delta);

    void set_rotation(const Rotation rotation);
    void move(const bool isMoving);
private:
    std::shared_ptr<RenderEngine::AnimatedSprite> sprite;

    float velocity;
    glm::vec2 position;
    glm::vec2 moveOffset;
    Rotation currentRotation;
    bool isMoving;
};

