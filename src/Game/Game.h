#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>
#include <memory>

#include "Tank.h"

class Game
{
public:
    explicit Game(const glm::ivec2 &windowSize);
    ~Game() = default;

    void render();
    void update(uint64_t delta);
    void set_key(int key, int action);
    bool init();

private:
    std::array<bool, 349> keys;

    enum class GameState{
        Active,
        Paused
    };

    GameState currentGameState;

    glm::ivec2 windowSize;

    std::unique_ptr<Tank> tank;
};
