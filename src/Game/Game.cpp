#include "Game.h"
#include "../Resources/ResourceManager.h"

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

    std::vector<std::string> subTexturesNames{
            "GreenTank0", "GreenTank1", "GreenTank2", "GreenTank3", "GreenTank4",
            "SingleBrick", "Bush", "Water", "SinglePlate", "Asphalt",
            "RedTank0", "RedTank1", "RedTank2", "RedTank3", "RedTank4",
            "DestroyedBrick", "Brick", "Plate", "Cup", "Box",
            "BlueTank0", "BlueTank1", "BlueTank2", "BlueTank3", "BlueTank4",
            "Heart", "HPBarLeft", "HPBarRight", "GreenLine", "RedLine",
            "SmallBullet0", "SmallBullet1", "SmallBullet2", "SmallBullet3", "SmallBullet4", "SmallBullet5",
            "GreenTankTower", "GreenTankWithoutTower0", "GreenTankWithoutTower1", "GreenTankWithoutTower2",
            "GreenTankWithoutTower3",
            "BigBullet0", "BigBullet1", "BigBullet2", "BigBullet3", "BigBullet4", "BigBullet5",
            "Empty1", "Empty2", "DestroyedGreenTank", "DestroyedRedTank", "DestroyedBlueTank"
    };

    auto textureAtlas = ResourceManager::load_textureAtlas("DefaultTextureAtlas", "res/textures/TankTownTiles.png",
                                                           subTexturesNames, 32, 32);

    auto animatedSprite = ResourceManager::load_animatedSprite("DefaultAnimatedSprite", "DefaultTextureAtlas",
                                                               "DefaultSpriteShader", 100.0f, 100.0f, "GreenTank0");
    animatedSprite->set_position(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> greenTankState;
    greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank0", 1000000000));
    greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank1", 1000000000));
    greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank2", 1000000000));
    greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank3", 1000000000));
    greenTankState.emplace_back(std::make_pair<std::string, uint64_t>("GreenTank4", 1000000000));

    animatedSprite->add_state("GreenTank", std::move(greenTankState));
    animatedSprite->set_state("GreenTank");

    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y), -100.0f, 100.0f);

    spriteShaderProgram->use();
    spriteShaderProgram->set_int("tex", 0);
    spriteShaderProgram->set_matrix4("projectionMat", projectionMatrix);

    return true;
}

void Game::render() {
    ResourceManager::get_animatedSprite("DefaultAnimatedSprite")->render();
}

void Game::update(const uint64_t delta) {
    ResourceManager::get_animatedSprite("DefaultAnimatedSprite")->update(delta);
}

void Game::set_key(const int key, const int action) {
    keys[key] = action;
}

