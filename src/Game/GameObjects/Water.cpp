#include "Water.h"
#include "../../Resources/ResourceManager.h"

Water::Water(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : IGameObject(ObjectType::Water, position, scale, rotation, layer),
          sprite(ResourceManager::get_sprite("water")),
          spriteAnimator(sprite),
          blockOffsets{glm::vec2(0, scale.y / 2), glm::vec2(scale.x / 2, scale.y / 2), glm::vec2(0),
                       glm::vec2(scale.x / 2, 0)} {
    colliders.emplace_back(glm::vec2(0), scale);
}

void Water::render_waterPart(Water::WaterPartLocation partLocation) const {
    sprite->render(position + blockOffsets[static_cast<size_t>(partLocation)],
                   scale / 2.0f, rotation, layer, spriteAnimator.get_currentFrame());
}

void Water::render() const {
    render_waterPart(WaterPartLocation::TopLeft);
    render_waterPart(WaterPartLocation::TopRight);
    render_waterPart(WaterPartLocation::BottomLeft);
    render_waterPart(WaterPartLocation::BottomRight);
}

void Water::update(double delta) {
    spriteAnimator.update(delta);
}

bool Water::collides(ObjectType type) {
    return type != IGameObject::ObjectType::Bullet;
}
