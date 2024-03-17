#pragma once

#include <array>
#include <cstdint>
#include <glm/vec2.hpp>
#include <memory>

#include "GameObjects/Tank.h"
#include "GameStates/Level.h"
#include "GameStates/StartScreen.h"

class Game
{
public:
    explicit Game(const glm::ivec2 &windowSize);

    ~Game() = default;

    void render();

    void update(double delta);

    bool init();

    void set_key(int key, int action);

    [[nodiscard]] unsigned int get_stateWidth() const;

    [[nodiscard]] unsigned int get_stateHeight() const;

private:
    std::array<bool, 349> keys;

    enum class GameState
    {
        StartScreen,
        LevelStart,
        Level,
        Paused,
        Scores,
        GameOver
    };

    GameState currentGameState;

    glm::ivec2 windowSize;

    std::shared_ptr<Tank> tank;
    std::shared_ptr<Level> level;
    std::shared_ptr<StartScreen> startScreen;
};
