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
    for (auto &currentObject: dynamicObjects) {
        if (currentObject->get_currentVelocity() > 0) {
            if (currentObject->get_currentDirection().x != 0.0f) {
                currentObject->get_currentPosition() = glm::vec2(currentObject->get_currentPosition().x,
                                                                 (float) static_cast<unsigned int>(
                                                                         currentObject->get_currentPosition().y / 4.0 +
                                                                         (double) 0.5) * 4.0f);
            } else if (currentObject->get_currentDirection().y != 0.0f) {
                currentObject->get_currentPosition() = glm::vec2(
                        (float) static_cast<unsigned int>(currentObject->get_currentPosition().x / 4.0f +
                                                          (double) 0.5f) * 4.0f,
                        currentObject->get_currentPosition().y);
            }

            const auto newPosition = currentObject->get_currentPosition() + currentObject->get_currentDirection() *
                                                                            static_cast<float>(
                                                                                    currentObject->get_currentVelocity() *
                                                                                    delta);
            const auto &colliders = currentObject->get_colliders();
            std::vector<std::shared_ptr<IGameObject>> objectsToCheck = currentLevel->get_objectsInArea(newPosition,
                                                                                                       newPosition +
                                                                                                       currentObject->get_scale());

            bool hasCollision = false;

            for (const auto &currentObjectToCheck: objectsToCheck) {
                const auto &collidersToCheck = currentObjectToCheck->get_colliders();

                if (!collidersToCheck.empty()) {
                    if (has_intersection(colliders, newPosition, collidersToCheck,
                                         currentObjectToCheck->get_currentPosition())) {
                        hasCollision = true;
                        break;
                    }
                }
            }

            if (!hasCollision) {
                currentObject->get_currentPosition() = newPosition;
            } else {
                if (currentObject->get_currentDirection().x != 0.0f) {
                    currentObject->get_currentPosition() = glm::vec2(
                            (float) static_cast<unsigned int>(currentObject->get_currentPosition().x / 8.0f +
                                                              (double) 0.5f) * 8.0f,
                            currentObject->get_currentPosition().y);
                } else if (currentObject->get_currentDirection().y != 0.0f) {
                    currentObject->get_currentPosition() = glm::vec2(currentObject->get_currentPosition().x,
                                                                     (float) static_cast<unsigned int>(
                                                                             currentObject->get_currentPosition().y /
                                                                             8.0f + (double) 0.5f) * 8.0f);
                }
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

bool PhysicsEngine::has_intersection(const std::vector<AABB> &lColliders, const glm::vec2 &lPosition,
                                     const std::vector<AABB> &rColliders, const glm::vec2 &rPosition) {
    for (const auto &currentLCollider: lColliders) {
        const glm::vec2 currentBottomLeftWorldLCollider = currentLCollider.bottomLeft + lPosition;
        const glm::vec2 currentTopRightWorldLCollider = currentLCollider.topRight + lPosition;

        for (const auto &currentRCollider: rColliders) {
            const glm::vec2 currentBottomLeftWorldRCollider = currentRCollider.bottomLeft + rPosition;
            const glm::vec2 currentTopRightWorldRCollider = currentRCollider.topRight + rPosition;

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
    }

    return true;
}
