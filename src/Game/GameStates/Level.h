#pragma once

#include <vector>
#include <memory>
#include "glm/vec2.hpp"
#include "../GameObjects/IGameObject.h"
#include "IGameState.h"
#include "../GameObjects/Tank.h"

class Level : public IGameState
{
public:
    static constexpr unsigned int BLOCK_SIZE = 16;

    explicit Level(const std::vector<std::string> &description);

    void render() const override;

    void update(double delta) override;

    void init_physics();

    void handle_input(std::array<bool, 349> &keys) override;

    [[nodiscard]] unsigned int get_width() const override;

    [[nodiscard]] unsigned int get_height() const override;

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
    std::shared_ptr<Tank> tank;
};
