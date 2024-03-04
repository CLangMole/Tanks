#include "Bush.h"
#include "../../Resources/ResourceManager.h"

Bush::Bush(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : IGameObject(position, scale, rotation, layer),
          sprite(ResourceManager::get_sprite("trees")),
          blockOffsets{glm::vec2(0, scale.y / 2),
                       glm::vec2(scale.x / 2, scale.y / 2), glm::vec2(0),
                       glm::vec2(scale.x / 2, 0)} {

}

void Bush::render_bushPart(Bush::BushPartLocation partLocation) const {
    sprite->render(position + blockOffsets[static_cast<size_t>(partLocation)],
                   scale / 2.0f, rotation, layer);
}

void Bush::render() const {
    render_bushPart(BushPartLocation::TopLeft);
    render_bushPart(BushPartLocation::TopRight);
    render_bushPart(BushPartLocation::BottomLeft);
    render_bushPart(BushPartLocation::BottomRight);
}
