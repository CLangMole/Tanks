#pragma once

#include <array>
#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

class Ice : public IGameObject
{
public:
    enum class IcePartLocation : uint8_t
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    Ice(const glm::vec2 &position, const glm::vec2 &scale, float rotation);

    void render() const override;

private:
    std::shared_ptr<RenderEngine::Sprite> sprite;
    std::array<glm::vec2, 4> blockOffsets;

    void render_icePart(IcePartLocation partLocation) const;
};
