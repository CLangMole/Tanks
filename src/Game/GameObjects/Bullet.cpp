#include "Bullet.h"
#include "../../Resources/ResourceManager.h"

Bullet::Bullet(double velocity, const glm::vec2 &position, const glm::vec2 &scale, const glm::vec2 &explosionScale,
               float layer)
        : IGameObject(ObjectType::Bullet, position, scale, 0.0f, layer),
          explosionScale(explosionScale), explosionOffset((this->explosionScale - this->scale) / 2.0f),
          spriteTop(ResourceManager::get_sprite("bullet_Top")),
          spriteBottom(ResourceManager::get_sprite("bullet_Bottom")),
          spriteLeft(ResourceManager::get_sprite("bullet_Left")),
          spriteRight(ResourceManager::get_sprite("bullet_Right")),
          spriteExplosion(ResourceManager::get_sprite("explosion")),
          explosionAnimator(this->spriteExplosion),
          isExploding(false),
          orientation(Rotation::Top), isActive(false), maxVelocity(velocity) {

    auto onCollisionCallback = [&](const IGameObject &object, const Physics::CollisionDirection direction) {
        set_velocity(0);
        isExploding = true;
        explosionTimer.start(explosionAnimator.get_totalDuration());
    };

    isExploding = true;
    explosionAnimator.reset();
    explosionTimer.start(explosionAnimator.get_totalDuration());

    colliders.emplace_back(glm::vec2(0), scale, onCollisionCallback);

    explosionTimer.on_complete([&]() {
        isExploding = false;
        isActive = false;
        explosionAnimator.reset();
    });
}

void Bullet::render() const {
    if (isExploding) {
        switch (orientation) {
            case Rotation::Top:
                spriteExplosion->render(position - explosionOffset + glm::vec2(0, scale.y / 2.0f), explosionScale,
                                        rotation, 0.0f, explosionAnimator.get_currentFrame());
                break;
            case Rotation::Bottom:
                spriteExplosion->render(position - explosionOffset - glm::vec2(0, scale.y / 2.0f), explosionScale,
                                        rotation, 0.0f, explosionAnimator.get_currentFrame());
                break;
            case Rotation::Left:
                spriteExplosion->render(position - explosionOffset - glm::vec2(0, scale.x / 2.0f), explosionScale,
                                        rotation, 0.0f, explosionAnimator.get_currentFrame());
                break;
            case Rotation::Right:
                spriteExplosion->render(position - explosionOffset + glm::vec2(0, scale.x / 2.0f), explosionScale,
                                        rotation, 0.0f, explosionAnimator.get_currentFrame());
                break;
        }
    } else if (isActive) {
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
}

void Bullet::update(double delta) {
    if (isExploding) {
        explosionAnimator.update(delta);
        explosionTimer.update(delta);
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
    set_velocity(maxVelocity);
}
