#include "BrickWall.h"
#include "../../Resources/ResourceManager.h"

BrickWall::BrickState BrickWall::get_collisionBrickState(const BrickWall::BrickState currentState,
                                                         const Physics::CollisionDirection direction) {
    switch (currentState) {
        case BrickState::All:
            switch (direction) {
                case Physics::CollisionDirection::Left:
                    return BrickState::Right;
                case Physics::CollisionDirection::Right:
                    return BrickState::Left;
                case Physics::CollisionDirection::Top:
                    return BrickState::Bottom;
                case Physics::CollisionDirection::Bottom:
                    return BrickState::Top;
            }

        case BrickState::Top:
            switch (direction) {
                case Physics::CollisionDirection::Left:
                    return BrickState::TopRight;
                case Physics::CollisionDirection::Right:
                    return BrickState::TopLeft;
                case Physics::CollisionDirection::Top:
                    return BrickState::Destroyed;
                case Physics::CollisionDirection::Bottom:
                    return BrickState::Destroyed;
            }

        case BrickState::Bottom:
            switch (direction) {
                case Physics::CollisionDirection::Left:
                    return BrickState::BottomRight;
                case Physics::CollisionDirection::Right:
                    return BrickState::BottomLeft;
                case Physics::CollisionDirection::Top:
                    return BrickState::Destroyed;
                case Physics::CollisionDirection::Bottom:
                    return BrickState::Destroyed;
            }

        case BrickState::Left:
            switch (direction) {
                case Physics::CollisionDirection::Left:
                    return BrickState::Destroyed;
                case Physics::CollisionDirection::Right:
                    return BrickState::Destroyed;
                case Physics::CollisionDirection::Top:
                    return BrickState::BottomLeft;
                case Physics::CollisionDirection::Bottom:
                    return BrickState::TopLeft;
            }

        case BrickState::Right:
            switch (direction) {
                case Physics::CollisionDirection::Left:
                    return BrickState::Destroyed;
                case Physics::CollisionDirection::Right:
                    return BrickState::Destroyed;
                case Physics::CollisionDirection::Top:
                    return BrickState::BottomRight;
                case Physics::CollisionDirection::Bottom:
                    return BrickState::TopRight;
            }

        default:
            return BrickState::Destroyed;
    }
}

Physics::AABB
BrickWall::get_AABBForBrickState(const BrickWall::BrickLocation location, const BrickWall::BrickState brickState,
                                 const glm::vec2 &scale) {
    glm::vec2 blockOffset(0);

    switch (location) {
        case BrickLocation::BottomLeft:
            break;
        case BrickLocation::BottomRight:
            blockOffset.x += scale.x / 2;
            break;
        case BrickLocation::TopLeft:
            blockOffset.y += scale.y / 2;
            break;
        case BrickLocation::TopRight:
            blockOffset += scale / 2.f;
            break;
    }

    glm::vec2 bottomLeft(0);
    glm::vec2 topRight(0);
    switch (brickState) {
        case BrickState::All:
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::TopLeft:
            bottomLeft = glm::vec2(0, scale.y / 4);
            topRight = glm::vec2(scale.x / 4, scale.y / 2);
            break;
        case BrickState::TopRight:
            bottomLeft = glm::vec2(scale.x / 4, scale.y / 4);
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::Top:
            bottomLeft = glm::vec2(0, scale.y / 4);
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::BottomLeft:
            topRight = glm::vec2(scale.x / 4, scale.y / 4);
            break;
        case BrickState::Left:
            topRight = glm::vec2(scale.x / 4, scale.y / 2);
            break;
        case BrickState::TopRight_BottomLeft:
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::Top_BottomLeft:
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::BottomRight:
            bottomLeft = glm::vec2(scale.x / 4, 0);
            topRight = glm::vec2(scale.x / 2, scale.y / 4);
            break;
        case BrickState::TopLeft_BottomRight:
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::Right:
            bottomLeft = glm::vec2(scale.x / 4, 0);
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::Top_BottomRight:
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::Bottom:
            topRight = glm::vec2(scale.x / 2, scale.y / 4);
            break;
        case BrickState::TopLeft_Bottom:
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::TopRight_Bottom:
            topRight = glm::vec2(scale.x / 2, scale.y / 2);
            break;
        case BrickState::Destroyed:
            break;
    }

    return {bottomLeft + blockOffset, topRight + blockOffset};
}

void BrickWall::on_collisionCallback(const BrickWall::BrickLocation location, const IGameObject &object,
                                     const Physics::CollisionDirection direction) {
    if (object.get_type() != IGameObject::ObjectType::Bullet) {
        return;
    }

    const BrickState newBrickState = get_collisionBrickState(currentBrickState[static_cast<size_t>(location)],
                                                             direction);
    currentBrickState[static_cast<size_t>(location)] = newBrickState;
    if (newBrickState != BrickState::Destroyed) {
        brickLocationToColliderMap[static_cast<size_t>(location)]->boundingBox = get_AABBForBrickState(location,
                                                                                                       newBrickState,
                                                                                                       scale);
    } else {
        brickLocationToColliderMap[static_cast<size_t>(location)]->isActive = false;
    }
}

BrickWall::BrickWall(WallType wallType, const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : IGameObject(ObjectType::Border, position, scale, rotation, layer),
          currentBrickState{BrickState::Destroyed, BrickState::Destroyed, BrickState::Destroyed,
                            BrickState::Destroyed},
          blockOffsets{glm::vec2(0, scale.y / 2), glm::vec2(scale.x / 2, scale.y / 2), glm::vec2(0),
                       glm::vec2(scale.x / 2, 0)},
          brickLocationToColliderMap{} {
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

    auto onCollisionCallbackTopLeft = [&](const IGameObject &object, const Physics::CollisionDirection direction) {
        on_collisionCallback(BrickLocation::TopLeft, object, direction);
    };
    auto onCollisionCallbackTopRight = [&](const IGameObject &object, const Physics::CollisionDirection direction) {
        on_collisionCallback(BrickLocation::TopRight, object, direction);
    };
    auto onCollisionCallbackBottomLeft = [&](const IGameObject &object, const Physics::CollisionDirection direction) {
        on_collisionCallback(BrickLocation::BottomLeft, object, direction);
    };
    auto onCollisionCallbackBottomRight = [&](const IGameObject &object, const Physics::CollisionDirection direction) {
        on_collisionCallback(BrickLocation::BottomRight, object, direction);
    };

    brickLocationToColliderMap.fill(nullptr);
    colliders.reserve(4);

    switch (wallType) {
        case WallType::All:
            currentBrickState.fill(BrickState::All);
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopLeft, BrickState::All, scale), onCollisionCallbackTopLeft));
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopRight, BrickState::All, scale),
                    onCollisionCallbackTopRight));
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomLeft, BrickState::All, scale),
                    onCollisionCallbackBottomLeft));
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomRight, BrickState::All, scale),
                    onCollisionCallbackBottomRight));
            break;
        case WallType::Top:
            currentBrickState[static_cast<size_t>(BrickLocation::TopLeft)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::TopRight)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopLeft, BrickState::All, scale), onCollisionCallbackTopLeft));
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopRight, BrickState::All, scale),
                    onCollisionCallbackTopRight));
            break;
        case WallType::Bottom:
            currentBrickState[static_cast<size_t>(BrickLocation::BottomLeft)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::BottomRight)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomLeft, BrickState::All, this->scale),
                    onCollisionCallbackBottomLeft));
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomRight, BrickState::All, scale),
                    onCollisionCallbackBottomRight));
            break;
        case WallType::Left:
            currentBrickState[static_cast<size_t>(BrickLocation::TopLeft)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::BottomLeft)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopLeft, BrickState::All, scale),
                    onCollisionCallbackTopLeft));
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomLeft, BrickState::All, scale),
                    onCollisionCallbackBottomLeft));
            break;
        case WallType::Right:
            currentBrickState[static_cast<size_t>(BrickLocation::TopRight)] = BrickState::All;
            currentBrickState[static_cast<size_t>(BrickLocation::BottomRight)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopRight, BrickState::All, scale),
                    onCollisionCallbackTopRight));
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomRight, BrickState::All, scale),
                    onCollisionCallbackBottomRight));
            break;
        case WallType::TopLeft:
            currentBrickState[static_cast<size_t>(BrickLocation::TopLeft)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopLeft, BrickState::All, scale),
                    onCollisionCallbackTopLeft));
            break;
        case WallType::TopRight:
            currentBrickState[static_cast<size_t>(BrickLocation::TopRight)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::TopRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::TopRight, BrickState::All, scale),
                    onCollisionCallbackTopRight));
            break;
        case WallType::BottomLeft:
            currentBrickState[static_cast<size_t>(BrickLocation::BottomLeft)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomLeft)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomLeft, BrickState::All, scale),
                    onCollisionCallbackBottomLeft));
            break;
        case WallType::BottomRight:
            currentBrickState[static_cast<size_t>(BrickLocation::BottomRight)] = BrickState::All;
            brickLocationToColliderMap[static_cast<size_t>(BrickLocation::BottomRight)] = &(colliders.emplace_back(
                    get_AABBForBrickState(BrickLocation::BottomRight, BrickState::All, scale),
                    onCollisionCallbackBottomRight));
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

