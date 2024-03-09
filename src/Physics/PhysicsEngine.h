#pragma once

#include <unordered_set>
#include <memory>
#include <glm/vec2.hpp>
#include <vector>

class IGameObject;

class Level;

namespace Physics
{
    struct AABB
    {
        AABB(const glm::vec2 &bottomLeft, const glm::vec2 &topRight) : bottomLeft(bottomLeft), topRight(topRight) {}

        glm::vec2 bottomLeft;
        glm::vec2 topRight;
    };

    class PhysicsEngine
    {
    public:

#pragma region

        PhysicsEngine() = delete;

        ~PhysicsEngine() = delete;

        PhysicsEngine(const PhysicsEngine &) = delete;

        PhysicsEngine(PhysicsEngine &&) = delete;

        PhysicsEngine &operator=(const PhysicsEngine &) = delete;

        PhysicsEngine &operator=(const PhysicsEngine &&) = delete;

#pragma endregion

        static void init();

        static void terminate();

        static void update(double delta);

        static void add_dynamicObject(std::shared_ptr<IGameObject> object);

        static void set_currentLevel(std::shared_ptr<Level> level);

    private:
        static std::unordered_set<std::shared_ptr<IGameObject>> dynamicObjects;
        static std::shared_ptr<Level> currentLevel;

        static bool has_intersection(const std::vector<AABB> &lColliders, const glm::vec2 &lPosition,
                                     const std::vector<AABB> &rColliders, const glm::vec2 &rPosition);
    };
}

