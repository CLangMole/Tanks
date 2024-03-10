#include "Eagle.h"
#include "../../Resources/ResourceManager.h"

Eagle::Eagle(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : IGameObject(position, scale, rotation, layer),
          sprites{ResourceManager::get_sprite("eagle"),
                  ResourceManager::get_sprite("eagle_dead")},
          currentState(EagleState::Alive) {
    colliders.emplace_back(glm::vec2(0), scale);
}

void Eagle::render() const {
    sprites[static_cast<size_t>(currentState)]->render(position, scale, rotation, layer);
}

void Eagle::update(double delta) {

}


