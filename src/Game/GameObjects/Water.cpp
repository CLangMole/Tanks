#include "Water.h"
#include "../../Resources/ResourceManager.h"

Water::Water(const glm::vec2 &position, const glm::vec2 &scale, float rotation)
        : IGameObject(position, scale, rotation),
          sprite(ResourceManager::get_sprite("water")),
          spriteAnimator(sprite),
          blockOffsets{glm::vec2(0, scale.y / 2), glm::vec2(scale.x / 2, scale.y / 2), glm::vec2(0),
                       glm::vec2(scale.x / 2, 0)} {

}

void Water::render_waterPart(Water::WaterPartLocation partLocation) const {
    sprite->render(position + blockOffsets[static_cast<size_t>(partLocation)],
                   scale / 2.0f, rotation, spriteAnimator.getCurrentFrame());
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
