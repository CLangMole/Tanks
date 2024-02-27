#include "Eagle.h"
#include "../../Resources/ResourceManager.h"

Eagle::Eagle(const glm::vec2 &position, const glm::vec2 &scale, float rotation)
        : IGameObject(position, scale, rotation),
          sprites{ResourceManager::get_sprite("eagle"),
                  ResourceManager::get_sprite("eagle_dead")},
          currentState(EagleState::Alive) {

}

void Eagle::render() const {
    sprites[static_cast<size_t>(currentState)]->render(position, scale, rotation);
}

void Eagle::update(double delta) {

}


