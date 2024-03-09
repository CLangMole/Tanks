#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(const glm::ivec2 &windowSize) : currentGameState(GameState::Active), windowSize(windowSize), keys(){
    keys.fill(false);
}

bool Game::init() {
    ResourceManager::load_json("res/resources.json");

    auto spriteShaderProgram = ResourceManager::get_shaderProgram("spriteShader");
    if (!spriteShaderProgram) {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

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
    if (tank) {
        tank->render();
    }

    if (level) {
        level->render();
    }
}

void Game::update(const double delta) {
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

        tank->update(delta);
    }
}

void Game::set_key(const int key, const int action) {
    keys[key] = action;
}

size_t Game::get_levelWidth() const {
    return level->get_width();
}

size_t Game::get_levelHeight() const {
    return level->get_height();
}

