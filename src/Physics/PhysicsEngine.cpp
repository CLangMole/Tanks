#include "PhysicsEngine.h"

#include "../Game/GameObjects/IGameObject.h"
#include "../Game/Level.h"

using namespace Physics;

std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::dynamicObjects;
std::shared_ptr<Level> PhysicsEngine::currentLevel;

void PhysicsEngine::init() {

}

void PhysicsEngine::terminate() {
    dynamicObjects.clear();
    currentLevel.reset();
}

void PhysicsEngine::update(double delta) {
    for (auto &dynamicObject: dynamicObjects) {
        if (dynamicObject->get_currentVelocity() > 0) {
            if (dynamicObject->get_currentDirection().x != 0.0f) {
                dynamicObject->get_currentPosition() = glm::vec2(dynamicObject->get_currentPosition().x,
                                                                 (float) static_cast<unsigned int>(
                                                                         dynamicObject->get_currentPosition().y / 4.0 +
                                                                         (double) 0.5) * 4.0f);
            } else if (dynamicObject->get_currentDirection().y != 0.0f) {
                dynamicObject->get_currentPosition() = glm::vec2(
                        (float) static_cast<unsigned int>(dynamicObject->get_currentPosition().x / 4.0f +
                                                          (double) 0.5f) * 4.0f,
                        dynamicObject->get_currentPosition().y);
            }

            const auto newPosition = dynamicObject->get_currentPosition() + dynamicObject->get_currentDirection() *
                                                                            static_cast<float>(
                                                                                    dynamicObject->get_currentVelocity() *
                                                                                    delta);
            const auto &colliders = dynamicObject->get_colliders();
            std::vector<std::shared_ptr<IGameObject>> objectsToCheck = currentLevel->get_objectsInArea(newPosition,
                                                                                                       newPosition +
                                                                                                       dynamicObject->get_scale());

            bool hasCollision = false;

            CollisionDirection dynamicObjectCollisionDirection = CollisionDirection::Right;
            if (dynamicObject->get_currentDirection().x < 0) {
                dynamicObjectCollisionDirection = CollisionDirection::Left;
            } else if (dynamicObject->get_currentDirection().y > 0) {
                dynamicObjectCollisionDirection = CollisionDirection::Top;
            } else if (dynamicObject->get_currentDirection().y < 0) {
                dynamicObjectCollisionDirection = CollisionDirection::Bottom;
            }

            CollisionDirection objectCollisionDirection = CollisionDirection::Left;
            if (dynamicObject->get_currentDirection().x < 0) {
                objectCollisionDirection = CollisionDirection::Right;
            } else if (dynamicObject->get_currentDirection().y > 0) {
                objectCollisionDirection = CollisionDirection::Bottom;
            } else if (dynamicObject->get_currentDirection().y < 0) {
                objectCollisionDirection = CollisionDirection::Top;
            }

            for (const auto &currentDynamicObjectCollider: colliders) {
                for (const auto &currentObjectToCheck: objectsToCheck) {
                    const auto &collidersToCheck = currentObjectToCheck->get_colliders();

                    if (currentObjectToCheck->collides(dynamicObject->get_type()) && !collidersToCheck.empty()) {
                        for (const auto &currentObjectCollider: currentObjectToCheck->get_colliders()) {
                            if (currentObjectCollider.isActive &&
                                has_intersection(currentDynamicObjectCollider, newPosition, currentObjectCollider,
                                                 currentObjectToCheck->get_currentPosition())) {
                                hasCollision = true;

                                if (currentObjectCollider.onCollisionCallback) {
                                    currentObjectCollider.onCollisionCallback(*dynamicObject, objectCollisionDirection);
                                }
                                if (currentDynamicObjectCollider.onCollisionCallback) {
                                    currentDynamicObjectCollider.onCollisionCallback(*currentObjectToCheck,
                                                                                     dynamicObjectCollisionDirection);
                                }
                            }
                        }
                    }
                }
            }

            if (!hasCollision) {
                dynamicObject->get_currentPosition() = newPosition;
            } else {
                if (dynamicObject->get_currentDirection().x != 0.0f) {
                    dynamicObject->get_currentPosition() = glm::vec2(
                            (float) static_cast<unsigned int>(dynamicObject->get_currentPosition().x / 8.0f +
                                                              (double) 0.5f) * 8.0f,
                            dynamicObject->get_currentPosition().y);
                } else if (dynamicObject->get_currentDirection().y != 0.0f) {
                    dynamicObject->get_currentPosition() = glm::vec2(dynamicObject->get_currentPosition().x,
                                                                     (float) static_cast<unsigned int>(
                                                                             dynamicObject->get_currentPosition().y /
                                                                             8.0f + (double) 0.5f) * 8.0f);
                }

                dynamicObject->on_collision();
            }
        }
    }
}

void PhysicsEngine::add_dynamicObject(std::shared_ptr<IGameObject> object) {
    dynamicObjects.insert(std::move(object));
}

void PhysicsEngine::set_currentLevel(std::shared_ptr<Level> level) {
    currentLevel.swap(level);
}

bool PhysicsEngine::has_intersection(const Collider &lCollider, const glm::vec2 &lPosition,
                                     const Collider &rCollider, const glm::vec2 &rPosition) {
    const glm::vec2 currentBottomLeftWorldLCollider = lCollider.boundingBox.bottomLeft + lPosition;
    const glm::vec2 currentTopRightWorldLCollider = lCollider.boundingBox.topRight + lPosition;

    const glm::vec2 currentBottomLeftWorldRCollider = rCollider.boundingBox.bottomLeft + rPosition;
    const glm::vec2 currentTopRightWorldRCollider = rCollider.boundingBox.topRight + rPosition;

    if (currentBottomLeftWorldLCollider.x >= currentTopRightWorldRCollider.x) {
        return false;
    }

    if (currentTopRightWorldLCollider.x <= currentBottomLeftWorldRCollider.x) {
        return false;
    }

    if (currentBottomLeftWorldLCollider.y >= currentTopRightWorldRCollider.y) {
        return false;
    }

    if (currentTopRightWorldLCollider.y <= currentBottomLeftWorldRCollider.y) {
        return false;
    }

    return true;
}
