#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>
#include <memory>

#include "Tank.h"

class Game
{
public:
    Game(const glm::ivec2 &windowSize);
    ~Game();

    void render();
    void update(const uint64_t delta);
    void set_key(const int key, const int action);
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
