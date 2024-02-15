#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "GLFW/glfw3.h"

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

Game::Game(const glm::ivec2 &windowSize) : currentGameState(GameState::Active), windowSize(windowSize){
    keys.fill(false);
}

Game::~Game() {

}

bool Game::init() {
    auto spriteShaderProgram = ResourceManager::load_shaders("DefaultSpriteShader", "res/shaders/vSprite.txt",
                                                             "res/shaders/fSprite.txt");

    if (!spriteShaderProgram) {
        std::cerr << "Can't create a shader program " << "DefaultSpriteShader" << std::endl;
        return false;
    }

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y), -100.0f, 100.0f);

    spriteShaderProgram->use();
    spriteShaderProgram->set_int("tex", 0);
    spriteShaderProgram->set_matrix4("projectionMat", projectionMatrix);

    std::vector<std::string> tankSubTexturesNames{
        "RedTankTop", "RedTankBottom", "RedTankRight", "RedTankLeft",
        "RedTankTop1", "RedTankBottom1", "RedTankRight1", "RedTankLeft1",

        "BlueTankTop", "BlueTankBottom", "BlueTankRight", "BlueTankLeft",
        "BlueTankTop1", "BlueTankBottom1", "BlueTankRight1", "BlueTankLeft1",

        "GreenTankTop", "GreenTankBottom", "GreenTankRight", "GreenTankLeft",
        "GreenTankTop1", "GreenTankBottom1", "GreenTankRight1", "GreenTankLeft1",

        "YellowTankTop", "YellowTankBottom", "YellowTankRight", "YellowTankLeft",
        "YellowTankTop1", "YellowTankBottom1", "YellowTankRight1", "YellowTankLeft1"
    };

    auto tanksTextures = ResourceManager::load_textureAtlas("TanksTextureAtlas", "res/textures/TanksTiles.png",
                                                            tankSubTexturesNames, 16, 16);
    auto tankSprite = ResourceManager::load_animatedSprite("GreenTank", "TanksTextureAtlas", "DefaultSpriteShader",
                                                           64.0f, 64.0f, "GreenTankTop");

#pragma region Tank states

    std::vector<std::pair<std::string, uint64_t>> greenTankTopState;
    greenTankTopState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankTop", 100000000));
    greenTankTopState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankTop1", 100000000));

    std::vector<std::pair<std::string, uint64_t>> greenTankBottomState;
    greenTankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankBottom", 100000000));
    greenTankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankBottom1", 100000000));

    std::vector<std::pair<std::string, uint64_t>> greenTankLeftState;
    greenTankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankLeft", 100000000));
    greenTankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankLeft1", 100000000));

    std::vector<std::pair<std::string, uint64_t>> greenTankRightState;
    greenTankRightState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankRight", 100000000));
    greenTankRightState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTankRight1", 100000000));

    tankSprite->add_state("TopState", std::move(greenTankTopState));
    tankSprite->add_state("BottomState", std::move(greenTankBottomState));
    tankSprite->add_state("LeftState", std::move(greenTankLeftState));
    tankSprite->add_state("RightState", std::move(greenTankRightState));

    tankSprite->set_state("TopState");

#pragma endregion

    tank = std::make_unique<Tank>(tankSprite, 0.0000001f, glm::vec2(100.0f, 100.0f));
    return true;
}

void Game::render() {
    if (!tank){
        std::cerr << "Can't find the tank" << std::endl;
        return;
    }

    tank->render();
}

void Game::update(const uint64_t delta) {
    if (!tank){
        std::cerr << "Can't find the tank" << std::endl;
        return;
    }

    if (keys[GLFW_KEY_W]){
        tank->set_rotation(Tank::Rotation::Top);
        tank->move(true);
    }
    else if (keys[GLFW_KEY_S]){
        tank->set_rotation(Tank::Rotation::Bottom);
        tank->move(true);
    }
    else if (keys[GLFW_KEY_A]){
        tank->set_rotation(Tank::Rotation::Left);
        tank->move(true);
    }
    else if (keys[GLFW_KEY_D]){
        tank->set_rotation(Tank::Rotation::Right);
        tank->move(true);
    }
    else{
        tank->move(false);
    }

    tank->update(delta);
}

void Game::set_key(const int key, const int action) {
    keys[key] = action;
}

