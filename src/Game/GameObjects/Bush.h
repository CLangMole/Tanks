#pragma once

#include <array>
#include <memory>
#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

class Bush : public IGameObject
{
public:
    enum class BushPartLocation : uint8_t
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    Bush(const glm::vec2 &position, const glm::vec2 &scale, float rotation);

    void render() const override;

private:
    std::shared_ptr<RenderEngine::Sprite> sprite;
    std::array<glm::vec2, 4> blockOffsets;

    void render_bushPart(BushPartLocation partLocation) const;
};