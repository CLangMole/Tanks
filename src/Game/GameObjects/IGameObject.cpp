#include "IGameObject.h"

IGameObject::IGameObject(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer)
        : position(position), scale(scale), rotation(rotation), layer(layer) {

}
