#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>
#include <memory>

#include "GameObjects/Tank.h"
#include "Level.h"

class Game
{
public:
    explicit Game(const glm::ivec2 &windowSize);

    ~Game() = default;

    void render();

    void update(double delta);

    bool init();

    void set_key(int key, int action);

    [[nodiscard]] size_t get_levelWidth() const;

    [[nodiscard]] size_t get_levelHeight() const;

private:
    std::array<bool, 349> keys;

    enum class GameState
    {
        Active,
        Paused
    };

    GameState currentGameState;

    glm::ivec2 windowSize;

    std::shared_ptr<Tank> tank;
    std::shared_ptr<Level> level;
};
