#include "Tank.h"
#include "../../Resources/ResourceManager.h"
#include <utility>

Tank::Tank(const float velocity, const glm::vec2 &position, const glm::vec2 &scale)
        : IGameObject(position, scale, 0.0f),
          spriteTop(ResourceManager::get_sprite("player2_green_tank_type1_sprite_top")),
          spriteBottom(ResourceManager::get_sprite("player2_green_tank_type1_sprite_bottom")),
          spriteLeft(ResourceManager::get_sprite("player2_green_tank_type1_sprite_left")),
          spriteRight(ResourceManager::get_sprite("player2_green_tank_type1_sprite_right")),
          spriteAnimatorTop(this->spriteTop), spriteAnimatorBottom(this->spriteBottom),
          spriteAnimatorLeft(this->spriteLeft), spriteAnimatorRight(this->spriteRight),
          velocity(velocity), currentRotation(Rotation::Top), isMoving(false),
          moveOffset(glm::vec2(0.0f, 1.0f)) {}

void Tank::render() const {
    switch (currentRotation) {
        case Rotation::Top:
            spriteTop->render(position, scale, rotation, spriteAnimatorTop.getCurrentFrame());
            break;
        case Rotation::Bottom:
            spriteBottom->render(position, scale, rotation, spriteAnimatorBottom.getCurrentFrame());
            break;
        case Rotation::Left:
            spriteLeft->render(position, scale, rotation, spriteAnimatorLeft.getCurrentFrame());
            break;
        case Rotation::Right:
            spriteRight->render(position, scale, rotation, spriteAnimatorRight.getCurrentFrame());
            break;
    }
}

void Tank::update(const double delta) {
    if (isMoving) {
        position += static_cast<float>(delta) * velocity * moveOffset * 100000.0f;

        switch (currentRotation) {
            case Rotation::Top:
                spriteAnimatorTop.update(delta);
                break;
            case Rotation::Bottom:
                spriteAnimatorBottom.update(delta);
                break;
            case Rotation::Left:
                spriteAnimatorLeft.update(delta);
                break;
            case Rotation::Right:
                spriteAnimatorRight.update(delta);
                break;
        }
    }
}

void Tank::set_rotation(const Rotation rotation) {
    if (this->currentRotation == rotation){
        return;
    }

    currentRotation = rotation;

    switch (currentRotation) {
        case Rotation::Top:
            moveOffset.x = 0;
            moveOffset.y = 1;

            break;
        case Rotation::Bottom:
            moveOffset.x = 0;
            moveOffset.y = -1;

            break;
        case Rotation::Left:
            moveOffset.x = -1;
            moveOffset.y = 0;

            break;
        case Rotation::Right:
            moveOffset.x = 1;
            moveOffset.y = 0;

            break;
    }
}

void Tank::move(const bool moving) {
    this->isMoving = moving;
}