#include "Tank.h"
#include "../../Resources/ResourceManager.h"

Tank::Tank(double maxVelocity, const glm::vec2 &position, const glm::vec2 &scale, float layer)
        : IGameObject(position, scale, 0.0f, layer),
          spriteTop(ResourceManager::get_sprite("player2_green_tank_type1_sprite_top")),
          spriteBottom(ResourceManager::get_sprite("player2_green_tank_type1_sprite_bottom")),
          spriteLeft(ResourceManager::get_sprite("player2_green_tank_type1_sprite_left")),
          spriteRight(ResourceManager::get_sprite("player2_green_tank_type1_sprite_right")),
          spriteRespawn(ResourceManager::get_sprite("respawn")), spriteShield(ResourceManager::get_sprite("shield")),
          spriteAnimatorTop(this->spriteTop), spriteAnimatorBottom(this->spriteBottom),
          spriteAnimatorLeft(this->spriteLeft), spriteAnimatorRight(this->spriteRight),
          spriteAnimatorRespawn(this->spriteRespawn), spriteAnimatorShield(this->spriteShield),
          maxVelocity(maxVelocity), currentRotation(Rotation::Top), isRespawning(true), hasShield(false) {
    respawnTimer.on_complete(
            [&]() {
                isRespawning = false;
                hasShield = true;
                shieldTimer.start(2000);
            }
    );

    shieldTimer.on_complete(
            [&]() {
                hasShield = false;
            }
    );

    respawnTimer.start(1500);

    colliders.emplace_back(glm::vec2(0), scale);
}

void Tank::render() const {
    if (isRespawning) {
        spriteRespawn->render(position, scale, rotation, layer, spriteAnimatorRespawn.get_currentFrame());
    } else {
        switch (currentRotation) {
            case Rotation::Top:
                spriteTop->render(position, scale, rotation, layer, spriteAnimatorTop.get_currentFrame());
                break;
            case Rotation::Bottom:
                spriteBottom->render(position, scale, rotation, layer, spriteAnimatorBottom.get_currentFrame());
                break;
            case Rotation::Left:
                spriteLeft->render(position, scale, rotation, layer, spriteAnimatorLeft.get_currentFrame());
                break;
            case Rotation::Right:
                spriteRight->render(position, scale, rotation, layer, spriteAnimatorRight.get_currentFrame());
                break;
        }

        if (hasShield) {
            spriteShield->render(position, scale, rotation, layer + 0.1f, spriteAnimatorShield.get_currentFrame());
        }
    }
}

void Tank::update(const double delta) {
    if (isRespawning) {
        spriteAnimatorRespawn.update(delta);
        respawnTimer.update(delta);
    } else {
        if (hasShield) {
            spriteAnimatorShield.update(delta);
            shieldTimer.update(delta);
        }

        if (currentVelocity > 0) {
            switch (currentRotation) {
                case Rotation::Top:
                    spriteAnimatorTop.update(delta * 3);
                    break;
                case Rotation::Bottom:
                    spriteAnimatorBottom.update(delta * 3);
                    break;
                case Rotation::Left:
                    spriteAnimatorLeft.update(delta * 3);
                    break;
                case Rotation::Right:
                    spriteAnimatorRight.update(delta * 3);
                    break;
            }
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
            direction.x = 0;
            direction.y = 1;

            break;
        case Rotation::Bottom:
            direction.x = 0;
            direction.y = -1;

            break;
        case Rotation::Left:
            direction.x = -1;
            direction.y = 0;

            break;
        case Rotation::Right:
            direction.x = 1;
            direction.y = 0;

            break;
    }
}

void Tank::set_velocity(double newVelocity) {
    if (!isRespawning) {
        currentVelocity = newVelocity;
    }
}

