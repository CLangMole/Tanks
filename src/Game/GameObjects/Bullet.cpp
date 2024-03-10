#include "Bullet.h"
#include "../../Resources/ResourceManager.h"

Bullet::Bullet(double velocity, const glm::vec2 &position, const glm::vec2 &scale, float layer)
        : IGameObject(position, scale, 0.0f, layer),
          spriteTop(ResourceManager::get_sprite("bullet_Top")),
          spriteBottom(ResourceManager::get_sprite("bullet_Bottom")),
          spriteLeft(ResourceManager::get_sprite("bullet_Left")),
          spriteRight(ResourceManager::get_sprite("bullet_Right")),
          orientation(Rotation::Top), isActive(false) {
    currentVelocity = velocity;
    colliders.emplace_back(glm::vec2(0), scale);
}

void Bullet::render() const {
    switch (orientation) {
        case Rotation::Top:
            spriteTop->render(position, scale, rotation, layer);
            break;
        case Rotation::Bottom:
            spriteBottom->render(position, scale, rotation, layer);
            break;
        case Rotation::Left:
            spriteLeft->render(position, scale, rotation, layer);
            break;
        case Rotation::Right:
            spriteRight->render(position, scale, rotation, layer);
            break;
    }
}

void Bullet::fire(const glm::vec2 &position, const glm::vec2 &direction) {
    this->position = position;
    this->direction = direction;

    if (this->direction.x == 0.0f) {
        orientation = direction.y < 0 ? Rotation::Bottom : Rotation::Top;
    } else {
        orientation = direction.x < 0 ? Rotation::Left : Rotation::Right;
    }

    isActive = true;
}
