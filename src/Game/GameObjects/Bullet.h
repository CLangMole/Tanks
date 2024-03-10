#pragma once

#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

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

    Bullet(double velocity, const glm::vec2 &position, const glm::vec2 &scale, float layer);

    void render() const override;

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
#pragma endregion

    Rotation orientation;
    bool isActive;
};
