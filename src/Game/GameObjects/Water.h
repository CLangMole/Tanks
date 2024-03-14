#pragma once

#include <memory>
#include <array>
#include "IGameObject.h"
#include "../../Renderer/Sprite.h"
#include "../../Renderer/SpriteAnimator.h"

class Water : public IGameObject
{
public:
    enum class WaterPartLocation : uint8_t
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    Water(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer);

    void render() const override;
    void update(double delta) override;

    bool collides(ObjectType type) override;
private:
    std::shared_ptr<RenderEngine::Sprite> sprite;
    RenderEngine::SpriteAnimator spriteAnimator;
    std::array<glm::vec2, 4> blockOffsets;

    void render_waterPart(WaterPartLocation partLocation) const;
};
