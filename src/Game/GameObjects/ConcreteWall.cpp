#include "ConcreteWall.h"
#include "../../Resources/ResourceManager.h"

ConcreteWall::ConcreteWall(WallType wallType, const glm::vec2 &position, const glm::vec2 &scale, float rotation,
                           float layer) : IGameObject(position, scale, rotation, layer),
                                          currentPartState{ConcretePartState::Destroyed,
                                                           ConcretePartState::Destroyed,
                                                           ConcretePartState::Destroyed,
                                                           ConcretePartState::Destroyed},
                                          blockOffsets{glm::vec2(0, scale.y / 2),
                                                       glm::vec2(scale.x / 2, scale.y / 2), glm::vec2(0),
                                                       glm::vec2(scale.x / 2, 0)},
                                          sprite(ResourceManager::get_sprite("concreteWall")) {
    switch (wallType) {
        case WallType::All:
            currentPartState.fill(ConcretePartState::Enabled);
            break;
        case WallType::Top:
            currentPartState[static_cast<size_t>(ConcretePartLocation::TopLeft)] = ConcretePartState::Enabled;
            currentPartState[static_cast<size_t>(ConcretePartLocation::TopRight)] = ConcretePartState::Enabled;
            break;
        case WallType::Bottom:
            currentPartState[static_cast<size_t>(ConcretePartLocation::BottomLeft)] = ConcretePartState::Enabled;
            currentPartState[static_cast<size_t>(ConcretePartLocation::BottomRight)] = ConcretePartState::Enabled;
            break;
        case WallType::Left:
            currentPartState[static_cast<size_t>(ConcretePartLocation::TopLeft)] = ConcretePartState::Enabled;
            currentPartState[static_cast<size_t>(ConcretePartLocation::BottomLeft)] = ConcretePartState::Enabled;
            break;
        case WallType::Right:
            currentPartState[static_cast<size_t>(ConcretePartLocation::TopRight)] = ConcretePartState::Enabled;
            currentPartState[static_cast<size_t>(ConcretePartLocation::BottomRight)] = ConcretePartState::Enabled;
            break;
        case WallType::TopLeft:
            currentPartState[static_cast<size_t>(ConcretePartLocation::TopLeft)] = ConcretePartState::Enabled;
            break;
        case WallType::TopRight:
            currentPartState[static_cast<size_t>(ConcretePartLocation::TopRight)] = ConcretePartState::Enabled;
            break;
        case WallType::BottomLeft:
            currentPartState[static_cast<size_t>(ConcretePartLocation::BottomLeft)] = ConcretePartState::Enabled;
            break;
        case WallType::BottomRight:
            currentPartState[static_cast<size_t>(ConcretePartLocation::BottomRight)] = ConcretePartState::Enabled;
            break;
    }
}

void ConcreteWall::render_concretePart(ConcreteWall::ConcretePartLocation partLocation) const {
    const ConcretePartState state = currentPartState[static_cast<size_t>(partLocation)];

    if (state != ConcretePartState::Destroyed) {
        sprite->render(position + blockOffsets[static_cast<size_t>(partLocation)],
                       scale / 2.0f, rotation, layer);
    }
}

void ConcreteWall::render() const {
    render_concretePart(ConcretePartLocation::TopLeft);
    render_concretePart(ConcretePartLocation::TopRight);
    render_concretePart(ConcretePartLocation::BottomLeft);
    render_concretePart(ConcretePartLocation::BottomRight);
}
