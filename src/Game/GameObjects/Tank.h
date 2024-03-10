#pragma once

#include <iostream>
#include <memory>
#include <glm/vec2.hpp>

#include "../../Renderer/Sprite.h"
#include "../../Renderer/SpriteAnimator.h"
#include "IGameObject.h"
#include "../../System/Timer.h"
#include "Bullet.h"

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

    Tank(double maxVelocity, const glm::vec2 &position, const glm::vec2 &scale, float layer);

    void render() const override;

    void update(double delta) override;

    void set_rotation(Rotation rotation);

    void set_velocity(double newVelocity) override;

    [[nodiscard]] double get_maxVelocity() const {
        return maxVelocity;
    };

    void fire();

private:

#pragma region Sprites
    std::shared_ptr<RenderEngine::Sprite> spriteTop;
    std::shared_ptr<RenderEngine::Sprite> spriteBottom;
    std::shared_ptr<RenderEngine::Sprite> spriteLeft;
    std::shared_ptr<RenderEngine::Sprite> spriteRight;

    std::shared_ptr<RenderEngine::Sprite> spriteRespawn;
    std::shared_ptr<RenderEngine::Sprite> spriteShield;
#pragma endregion

#pragma region Sprites animators
    RenderEngine::SpriteAnimator spriteAnimatorTop;
    RenderEngine::SpriteAnimator spriteAnimatorBottom;
    RenderEngine::SpriteAnimator spriteAnimatorLeft;
    RenderEngine::SpriteAnimator spriteAnimatorRight;

    RenderEngine::SpriteAnimator spriteAnimatorRespawn;
    RenderEngine::SpriteAnimator spriteAnimatorShield;
#pragma endregion

    double maxVelocity;
    Rotation currentRotation;

    bool isRespawning;
    bool hasShield;

    Timer respawnTimer;
    Timer shieldTimer;

    std::shared_ptr<Bullet> currentBullet;
};

