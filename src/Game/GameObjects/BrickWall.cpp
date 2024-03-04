#include "BrickWall.h"
#include "../../Resources/ResourceManager.h"

BrickWall::BrickWall(WallType wallType, const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : IGameObject(position, scale, rotation, layer),
          currentBrickState{BrickState::Destroyed, BrickState::Destroyed, BrickState::Destroyed,
                            BrickState::Destroyed},
          blockOffsets{glm::vec2(0, scale.y / 2), glm::vec2(scale.x / 2, scale.y / 2), glm::vec2(0),
                       glm::vec2(scale.x / 2, 0)} {
    sprites[static_cast<size_t>(BrickState::All)] = ResourceManager::get_sprite("brickWall_All");
    sprites[static_cast<size_t>(BrickState::TopLeft)] = ResourceManager::get_sprite("brickWall_TopLeft");
    sprites[static_cast<size_t>(BrickState::TopRight)] = ResourceManager::get_sprite("brickWall_TopRight");
    sprites[static_cast<size_t>(BrickState::Top)] = ResourceManager::get_sprite("brickWall_Top");
    sprites[static_cast<size_t>(BrickState::BottomLeft)] = ResourceManager::get_sprite("brickWall_BottomLeft");
    sprites[static_cast<size_t>(BrickState::Left)] = ResourceManager::get_sprite("brickWall_Left");
    sprites[static_cast<size_t>(BrickState::TopRight_BottomLeft)] = ResourceManager::get_sprite(
            "brickWall_TopRight_BottomLeft");
    sprites[static_cast<size_t>(BrickState::Top_BottomLeft)] = ResourceManager::get_sprite(
            "brickWall_Top_BottomLeft");
    sprites[static_cast<size_t>(BrickState::BottomRight)] = ResourceManager::get_sprite("brickWall_BottomRight");
    sprites[static_cast<size_t>(BrickState::TopLeft_BottomRight)] = ResourceManager::get_sprite(
            "brickWall_TopLeft_BottomRight");
    sprites[static_cast<size_t>(BrickState::Right)] = ResourceManager::get_sprite("brickWall_Right");
    sprites[static_cast<size_t>(BrickState::Top_BottomRight)] = ResourceManager::get_sprite(
            "brickWall_Top_BottomRight");
    sprites[static_cast<size_t>(BrickState::Bottom)] = ResourceManager::get_sprite("brickWall_Bottom");
    sprites[static_cast<size_t>(BrickState::TopLeft_Bottom)] = ResourceManager::get_sprite(
            "brickWall_TopLeft_Bottom");
    sprites[static_cast<size_t>(BrickState::TopRight_Bottom)] = ResourceManager::get_sprite(
            "brickWall_TopRight_Bottom");

    switch (wallType) {
        case WallType::All:
            currentBrickState.fill(BrickState::All);
            break;
        case WallType::Top:
            currentBrickState[static_cast<size_t>(BrickLocation::TopLeft)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::TopRight)] = BrickState::All;
            break;
        case WallType::Bottom:
            currentBrickState[static_cast<size_t>(BrickLocation::BottomLeft)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::BottomRight)] = BrickState::All;
            break;
        case WallType::Left:
            currentBrickState[static_cast<size_t>(BrickLocation::TopLeft)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::BottomLeft)] = BrickState::All;
            break;
        case WallType::Right:
            currentBrickState[static_cast<size_t>(BrickLocation::TopRight)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::BottomRight)] = BrickState::All;
            break;
        case WallType::TopLeft:
            currentBrickState[static_cast<size_t>(BrickLocation::TopLeft)] = BrickState::All;
            break;
        case WallType::TopRight:
            currentBrickState[static_cast<size_t>(BrickLocation::TopRight)] = BrickState::All;
            break;
        case WallType::BottomLeft:
            currentBrickState[static_cast<size_t>(BrickLocation::BottomLeft)] = BrickState::All;
            break;
        case WallType::BottomRight:
            currentBrickState[static_cast<size_t>(BrickLocation::BottomRight)] = BrickState::All;
            break;
    }
}

void BrickWall::render_brick(const BrickLocation brickLocation) const {
    const BrickState state = currentBrickState[static_cast<size_t>(brickLocation)];

    if (state != BrickState::Destroyed) {
        sprites[static_cast<size_t>(state)]->render(position + blockOffsets[static_cast<size_t>(brickLocation)],
                                                    scale / 2.0f, rotation, layer);
    }
}

void BrickWall::render() const {
    render_brick(BrickLocation::TopLeft);
    render_brick(BrickLocation::TopRight);
    render_brick(BrickLocation::BottomLeft);
    render_brick(BrickLocation::BottomRight);
}

