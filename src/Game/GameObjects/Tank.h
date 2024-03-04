#pragma once

#include <iostream>
#include <memory>
#include <glm/vec2.hpp>

#include "../../Renderer/Sprite.h"
#include "../../Renderer/SpriteAnimator.h"
#include "IGameObject.h"

class Tank : public IGameObject
{
public:
#pragma region
    enum class Rotation : uint8_t
    {
        Top,
        Bottom,
        Left,
        Right
    };
#pragma endregion

    Tank(float velocity, const glm::vec2 &position, const glm::vec2 &scale, float layer);

    void render() const override;

    void update(double delta) override;

    void set_rotation(Rotation rotation);

    void move(bool moving);

private:
    std::shared_ptr<RenderEngine::Sprite> spriteTop;
    std::shared_ptr<RenderEngine::Sprite> spriteBottom;
    std::shared_ptr<RenderEngine::Sprite> spriteLeft;
    std::shared_ptr<RenderEngine::Sprite> spriteRight;

    RenderEngine::SpriteAnimator spriteAnimatorTop;
    RenderEngine::SpriteAnimator spriteAnimatorBottom;
    RenderEngine::SpriteAnimator spriteAnimatorLeft;
    RenderEngine::SpriteAnimator spriteAnimatorRight;

    float velocity;
    glm::vec2 moveOffset;
    Rotation currentRotation;
    bool isMoving;
};

