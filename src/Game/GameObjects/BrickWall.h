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

    BrickWall(WallType wallType, const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer);

    void render() const override;

private:
    void render_brick(BrickLocation brickLocation) const;

    static BrickState get_collisionBrickState(BrickState currentState, Physics::CollisionDirection direction);

    static Physics::AABB get_AABBForBrickState(BrickLocation location, BrickState brickState, const glm::vec2 &scale);

    void on_collisionCallback(BrickLocation location, const IGameObject &object, Physics::CollisionDirection direction);

    std::array<Physics::Collider *, 4> brickLocationToColliderMap;

    std::array<BrickState, 4> currentBrickState;
    std::array<std::shared_ptr<RenderEngine::Sprite>, 15> sprites;
    std::array<glm::vec2, 4> blockOffsets;
};
