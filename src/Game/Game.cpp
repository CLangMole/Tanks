#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(const glm::ivec2 &windowSize) : currentGameState(GameState::StartScreen), windowSize(windowSize), keys() {
    keys.fill(false);
}

bool Game::init() {
    ResourceManager::load_json("res/resources.json");

    auto spriteShaderProgram = ResourceManager::get_shaderProgram("spriteShader");
    if (!spriteShaderProgram) {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

    startScreen = std::make_shared<StartScreen>(ResourceManager::get_startScreen());
    level = std::make_shared<Level>(ResourceManager::get_levels()[0]); //TODO Fix depth test issue
    windowSize.x = static_cast<int>(level->get_width()); //TODO Fix viewport
    windowSize.y = static_cast<int>(level->get_height());
    Physics::PhysicsEngine::set_currentLevel(level);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f,
                                            static_cast<float>(windowSize.y), -100.0f, 100.0f);

    spriteShaderProgram->use();
    spriteShaderProgram->set_int("tex", 0);
    spriteShaderProgram->set_matrix4("projectionMat", projectionMatrix);

    tank = std::make_shared<Tank>(0.05f, level->getPlayerRespawn1(),
                                  glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.0f);
    Physics::PhysicsEngine::add_dynamicObject(tank);

    return true;
}

void Game::render() {
    switch (currentGameState) {
        case GameState::StartScreen:
            startScreen->render();
            break;
        case GameState::Level:
            if (tank) {
                tank->render();
            }

            if (level) {
                level->render();
            }
            break;
        case GameState::LevelStart:
            break;
        case GameState::Paused:
            break;
        case GameState::Scores:
            break;
        case GameState::GameOver:
            break;
    }
}

void Game::update(const double delta) {
    switch (currentGameState) {
        case GameState::StartScreen:
            if (keys[GLFW_KEY_ENTER]) {
                currentGameState = GameState::Level;
            }
            break;
        case GameState::LevelStart:
            break;
        case GameState::Level:
            if (level) {
                level->update(delta);
            }

            if (tank) {
                if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
                    tank->set_rotation(Tank::Rotation::Top);
                    tank->set_velocity(tank->get_maxVelocity());
                } else if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
                    tank->set_rotation(Tank::Rotation::Bottom);
                    tank->set_velocity(tank->get_maxVelocity());
                } else if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
                    tank->set_rotation(Tank::Rotation::Left);
                    tank->set_velocity(tank->get_maxVelocity());
                } else if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
                    tank->set_rotation(Tank::Rotation::Right);
                    tank->set_velocity(tank->get_maxVelocity());
                } else {
                    tank->set_velocity(0);
                }

                if (tank && keys[GLFW_KEY_SPACE]) {
                    tank->fire();
                }

                tank->update(delta);
            }

            break;
        case GameState::Paused:
            break;
        case GameState::Scores:
            break;
        case GameState::GameOver:
            break;
    }
}

void Game::set_key(const int key, const int action) {
    keys[key] = action;
}

unsigned int Game::get_stateWidth() const {
    switch (currentGameState) {
        case GameState::StartScreen:
            return startScreen->get_width();
        case GameState::Level:
            return level->get_width();
        case GameState::LevelStart:
            return 0;
        case GameState::Paused:
            return 0;
        case GameState::Scores:
            return 0;
        case GameState::GameOver:
            return 0;
    }

    return 0;
}

unsigned int Game::get_stateHeight() const {
    switch (currentGameState) {
        case GameState::StartScreen:
            return startScreen->get_height();
        case GameState::Level:
            return level->get_height();
        case GameState::LevelStart:
            return 0;
        case GameState::Paused:
            return 0;
        case GameState::Scores:
            return 0;
        case GameState::GameOver:
            return 0;
    }

    return 0;
}

