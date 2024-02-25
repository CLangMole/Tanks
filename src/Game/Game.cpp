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

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f,
                                            static_cast<float>(windowSize.y), -100.0f, 100.0f);

    spriteShaderProgram->use();
    spriteShaderProgram->set_int("tex", 0);
    spriteShaderProgram->set_matrix4("projectionMat", projectionMatrix);

    tank = std::make_unique<Tank>(0.0000001f, glm::vec2(0), glm::vec2(16.0f, 16.0f));
    level = std::make_unique<Level>(ResourceManager::get_levels()[0]);

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
        if (keys[GLFW_KEY_W]) {
            tank->set_rotation(Tank::Rotation::Top);
            tank->move(true);
        } else if (keys[GLFW_KEY_S]) {
            tank->set_rotation(Tank::Rotation::Bottom);
            tank->move(true);
        } else if (keys[GLFW_KEY_A]) {
            tank->set_rotation(Tank::Rotation::Left);
            tank->move(true);
        } else if (keys[GLFW_KEY_D]) {
            tank->set_rotation(Tank::Rotation::Right);
            tank->move(true);
        } else {
            tank->move(false);
        }

        tank->update(delta);
    }
}

void Game::set_key(const int key, const int action) {
    keys[key] = action;
}

