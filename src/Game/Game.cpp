#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "GLFW/glfw3.h"
#include "../Renderer/Renderer.h"

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(const glm::uvec2 &windowSize) : gameState(GameState::StartScreen), windowSize(windowSize), keys() {
    keys.fill(false);
}

bool Game::init() {
    ResourceManager::load_json("res/resources.json");

    spriteShader = ResourceManager::get_shaderProgram("spriteShader");
    if (!spriteShader) {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

    currentGameState = std::make_shared<StartScreen>(ResourceManager::get_startScreen());

    spriteShader->use();
    spriteShader->set_int("tex", 0);

    set_windowSize(windowSize);

    //TODO Fix viewport
    //TODO Fix depth test issue

    return true;
}

void Game::render() {
    currentGameState->render();
}

void Game::update(const double delta) {
    switch (gameState) {
        case GameState::StartScreen:
            if (keys[GLFW_KEY_ENTER]) {
                gameState = GameState::Level;
                start_level(0);
            }
            break;
        case GameState::Level:
            currentGameState->handle_input(keys);
            currentGameState->update(delta);
            break;
    }
}

void Game::start_level(size_t index) {
    auto level = std::make_shared<Level>(ResourceManager::get_levels()[index]);
    currentGameState = level;
    Physics::PhysicsEngine::set_currentLevel(level);
    update_viewport();
}

void Game::set_windowSize(const glm::uvec2 &startWindowSize) {
    windowSize = startWindowSize;
    update_viewport();
}

void Game::update_viewport() const {
    const float aspectRatio = static_cast<float>(get_stateWidth()) / static_cast<float>(get_stateHeight());

    unsigned int viewportWidth = windowSize.x;
    unsigned int viewportHeight = windowSize.y;

    unsigned int leftOffset = 0;
    unsigned int bottomOffset = 0;

    if (static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y) > aspectRatio) {
        viewportWidth = static_cast<unsigned int>(static_cast<float>(windowSize.y) * aspectRatio);
        leftOffset = (windowSize.x - viewportWidth) / 2;
    } else {
        viewportHeight = static_cast<unsigned int>(static_cast<float>(windowSize.x) / aspectRatio);
        bottomOffset = (windowSize.y - viewportHeight) / 2;
    }

    RenderEngine::Renderer::set_viewport(static_cast<int>(viewportWidth), static_cast<int>(viewportHeight),
                                         static_cast<int>(leftOffset), static_cast<int>(bottomOffset));

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(get_stateWidth()), 0.0f,
                                            static_cast<float>(get_stateHeight()), -100.0f, 100.0f);

    spriteShader->set_matrix4("projectionMat", projectionMatrix);
}

void Game::set_key(const int key, const int action) {
    keys[key] = action;
}

unsigned int Game::get_stateWidth() const {
    return currentGameState->get_width();
}

unsigned int Game::get_stateHeight() const {
    return currentGameState->get_height();
}

