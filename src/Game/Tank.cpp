#include "Tank.h"

Tank::Tank(std::shared_ptr<RenderEngine::AnimatedSprite> sprite, const float velocity, const glm::vec2 &position)
        : sprite(std::move(sprite)), velocity(velocity), position(position),
          currentRotation(Rotation::Top), isMoving(false), moveOffset(glm::vec2(0.0f, 1.0f)) {
    this->sprite->set_position(position);
}

void Tank::render() const {
    sprite->render();
}

void Tank::update(const uint64_t delta) {
    if (isMoving){
        position += delta * velocity * moveOffset;
        sprite->set_position(position);
        sprite->update(delta);
    }
}

void Tank::set_rotation(const Rotation rotation) {
    if (this->currentRotation == rotation){
        return;
    }

    currentRotation = rotation;

    switch (currentRotation) {
        default:
            break;
        case Rotation::Top:
            sprite->set_state("TopState");

            moveOffset.x = 0;
            moveOffset.y = 1;

            break;
        case Rotation::Bottom:
            sprite->set_state("BottomState");

            moveOffset.x = 0;
            moveOffset.y = -1;

            break;
        case Rotation::Left:
            sprite->set_state("LeftState");

            moveOffset.x = -1;
            moveOffset.y = 0;

            break;
        case Rotation::Right:
            sprite->set_state("RightState");

            moveOffset.x = 1;
            moveOffset.y = 0;

            break;
    }
}

void Tank::move(const bool isMoving) {
    this->isMoving = isMoving;
}