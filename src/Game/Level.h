#pragma once

#include <vector>
#include <memory>
#include "..//Game/GameObjects/IGameObject.h"
#include <glm/vec2.hpp>

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

private:
    size_t width;
    size_t height;

    glm::ivec2 playerRespawn1{};
    glm::ivec2 playerRespawn2{};

    std::vector<std::shared_ptr<IGameObject>> gameObjects;
};
