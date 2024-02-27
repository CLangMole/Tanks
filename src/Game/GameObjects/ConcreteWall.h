#pragma once

#include <array>
#include <memory>
#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

class ConcreteWall : public IGameObject
{
public:
#pragma region States
    enum class WallType : uint8_t
    {
        All,
        Top, Bottom,
        Left, Right,
        TopLeft, TopRight,
        BottomLeft, BottomRight
    };

    enum class ConcretePartState : uint8_t
    {
        Enabled = 0,
        Destroyed
    };

    enum class ConcretePartLocation : uint8_t
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };
#pragma endregion

    ConcreteWall(WallType wallType, const glm::vec2 &position, const glm::vec2 &scale, float rotation);

    void render() const override;

private:
    std::array<ConcretePartState, 4> currentPartState;
    std::shared_ptr<RenderEngine::Sprite> sprite;
    std::array<glm::vec2, 4> blockOffsets;

    void render_concretePart(ConcretePartLocation partLocation) const;
};
