#pragma once

#include <unordered_set>
#include <memory>
#include <utility>
#include <glm/vec2.hpp>
#include <vector>
#include <functional>

class IGameObject;

class Level;

namespace Physics
{
    enum class CollisionDirection : uint8_t
    {
        Top,
        Bottom,
        Left,
        Right
    };

    struct AABB
    {
        AABB(const glm::vec2 &bottomLeft, const glm::vec2 &topRight) : bottomLeft(bottomLeft), topRight(topRight) {}

        glm::vec2 bottomLeft;
        glm::vec2 topRight;
    };

    struct Collider
    {
        Collider(const glm::vec2 &bottomLeft, const glm::vec2 &topRight,
                 std::function<void(const IGameObject &, const CollisionDirection)> callback = {})
                : boundingBox(bottomLeft, topRight), onCollisionCallback(std::move(callback)), isActive(true) {}

        explicit Collider(const AABB &boundingBox,
                          std::function<void(const IGameObject &, const CollisionDirection)> callback = {})
                : boundingBox(boundingBox), onCollisionCallback(std::move(callback)), isActive(true) {}

        AABB boundingBox;
        bool isActive;
        std::function<void(const IGameObject &, const CollisionDirection)> onCollisionCallback;
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

        static bool has_intersection(const Collider &lCollider, const glm::vec2 &lPosition,
                                     const Collider &rCollider, const glm::vec2 &rPosition);
    };
}

