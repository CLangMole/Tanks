#include "Border.h"
#include "../../Resources/ResourceManager.h"

Border::Border(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : IGameObject(position, scale, rotation, layer),
          sprite(ResourceManager::get_sprite("border")) {

}

void Border::render() const {
    sprite->render(position, scale, rotation, layer);
}
