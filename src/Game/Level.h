#pragma once

#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include "GameObjects/IGameObject.h"

class Level
{
public:
    static constexpr unsigned int BLOCK_SIZE = 16;

    explicit Level(const std::vector<std::string> &description);

    void render() const;

    void update(double delta);

    [[nodiscard]] size_t get_width() const;

    [[nodiscard]] size_t get_height() const;

    [[nodiscard]] const glm::ivec2 &getPlayerRespawn1() const {
        return playerRespawn1;
    }

    [[nodiscard]] const glm::ivec2 &getPlayerRespawn2() const {
        return playerRespawn2;
    }

    [[nodiscard]] std::vector<std::shared_ptr<IGameObject>>
    get_objectsInArea(const glm::vec2 &bottomLeft, const glm::vec2 &topRight) const;

private:
    size_t widthBlocks;
    size_t heightBlocks;

    unsigned int widthPixels = 0;
    unsigned int heightPixels = 0;

    glm::ivec2 playerRespawn1{};
    glm::ivec2 playerRespawn2{};

    std::vector<std::shared_ptr<IGameObject>> gameObjects;
};
