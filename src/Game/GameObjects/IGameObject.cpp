#include "IGameObject.h"

IGameObject::IGameObject(ObjectType objectType, const glm::vec2 &position, const glm::vec2 &scale, float rotation,
                         float layer)
        : position(position), scale(scale), rotation(rotation), layer(layer), direction(0, 1), currentVelocity(0),
          objectType(objectType) {

}

void IGameObject::set_velocity(double newVelocity) {
    currentVelocity = newVelocity;
}
