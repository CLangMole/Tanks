#include "Ice.h"
#include "../../Resources/ResourceManager.h"

Ice::Ice(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : IGameObject(position, scale, rotation, layer),
          sprite(ResourceManager::get_sprite("ice")),
          blockOffsets{glm::vec2(0, scale.y / 2),
                       glm::vec2(scale.x / 2, scale.y / 2), glm::vec2(0),
                       glm::vec2(scale.x / 2, 0)} {

}

void Ice::render_icePart(Ice::IcePartLocation partLocation) const {
    sprite->render(position + blockOffsets[static_cast<size_t>(partLocation)],
                   scale / 2.0f, rotation, layer);
}

void Ice::render() const {
    render_icePart(IcePartLocation::TopLeft);
    render_icePart(IcePartLocation::TopRight);
    render_icePart(IcePartLocation::BottomLeft);
    render_icePart(IcePartLocation::BottomRight);
}
