#pragma once

#include "IGameObject.h"
#include "../../Renderer/Sprite.h"
#include "../../Renderer/SpriteAnimator.h"
#include "../../System/Timer.h"

class Bullet : public IGameObject
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

    Bullet(double velocity, const glm::vec2 &position, const glm::vec2 &scale, const glm::vec2 &explosionScale,
           float layer);

    void render() const override;

    void update(double delta) override;

    [[nodiscard]] bool is_active() const {
        return isActive;
    }

    void fire(const glm::vec2 &position, const glm::vec2 &direction);

private:
#pragma region Sprites
    std::shared_ptr<RenderEngine::Sprite> spriteTop;
    std::shared_ptr<RenderEngine::Sprite> spriteBottom;
    std::shared_ptr<RenderEngine::Sprite> spriteLeft;
    std::shared_ptr<RenderEngine::Sprite> spriteRight;

    std::shared_ptr<RenderEngine::Sprite> spriteExplosion;
    RenderEngine::SpriteAnimator explosionAnimator;
#pragma endregion

    Rotation orientation;
    bool isActive;
    double maxVelocity;

    glm::vec2 explosionScale;
    glm::vec2 explosionOffset;
    bool isExploding;
    Timer explosionTimer;
};
