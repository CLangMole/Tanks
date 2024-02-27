#pragma once

#include <memory>
#include <array>
#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

class BrickWall : public IGameObject
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

    enum class BrickState : uint8_t
    {
        All = 0,
        TopLeft,
        TopRight,
        Top,
        BottomLeft,
        Left,
        TopRight_BottomLeft,
        Top_BottomLeft,
        BottomRight,
        TopLeft_BottomRight,
        Right,
        Top_BottomRight,
        Bottom,
        TopLeft_Bottom,
        TopRight_Bottom,
        Destroyed
    };

    enum class BrickLocation : uint8_t
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };
#pragma endregion

    BrickWall(WallType wallType, const glm::vec2 &position, const glm::vec2 &scale, float rotation);

    void render() const override;
private:
    std::array<BrickState, 4> currentBrickState;
    std::array<std::shared_ptr<RenderEngine::Sprite>, 15> sprites;
    std::array<glm::vec2, 4> blockOffsets;

    void render_brick(BrickLocation brickLocation) const;
};
