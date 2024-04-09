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
    explicit Game(const glm::uvec2 &windowSize);

    ~Game() = default;

    void render();

    void update(double delta);

    bool init();

    void set_key(int key, int action);

    void set_windowSize(const glm::uvec2 &startWindowSize);

    [[nodiscard]] unsigned int get_stateWidth() const;

    [[nodiscard]] unsigned int get_stateHeight() const;

private:
    std::array<bool, 349> keys;

    void start_level(size_t index);

    void update_viewport() const;

    enum class GameState
    {
        StartScreen,
        Level
    };

    GameState gameState;
    glm::uvec2 windowSize;

    std::shared_ptr<IGameState> currentGameState;
    std::shared_ptr<RenderEngine::ShaderProgram> spriteShader;
};
