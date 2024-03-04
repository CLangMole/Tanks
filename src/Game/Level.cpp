#include "Level.h"

#include "GameObjects/BrickWall.h"
#include "GameObjects/ConcreteWall.h"
#include "GameObjects/Bush.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"

#include <iostream>

std::shared_ptr<IGameObject> createFromDescription(const char description,
                                                   const glm::vec2 &position, const glm::vec2 &scale,
                                                   const float rotation) {
    switch (description) {
        case '0':
            return std::make_shared<BrickWall>(BrickWall::WallType::Right, position, scale,
                                               rotation, 0.0f);
        case '1':
            return std::make_shared<BrickWall>(BrickWall::WallType::Bottom, position, scale,
                                               rotation, 0.0f);
        case '2':
            return std::make_shared<BrickWall>(BrickWall::WallType::Left, position, scale,
                                               rotation, 0.0f);
        case '3':
            return std::make_shared<BrickWall>(BrickWall::WallType::Top, position, scale,
                                               rotation, 0.0f);
        case '4':
            return std::make_shared<BrickWall>(BrickWall::WallType::All, position, scale,
                                               rotation, 0.0f);
        case 'G':
            return std::make_shared<BrickWall>(BrickWall::WallType::BottomLeft, position, scale,
                                               rotation, 0.0f);
        case 'H':
            return std::make_shared<BrickWall>(BrickWall::WallType::BottomRight, position, scale,
                                               rotation, 0.0f);
        case 'I':
            return std::make_shared<BrickWall>(BrickWall::WallType::TopLeft, position, scale,
                                               rotation, 0.0f);
        case 'J':
            return std::make_shared<BrickWall>(BrickWall::WallType::TopRight, position, scale,
                                               rotation, 0.0f);
        case '5':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Right, position, scale,
                                                  rotation, 0.0f);
        case '6':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Bottom, position, scale,
                                                  rotation, 0.0f);
        case '7':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Left, position, scale,
                                                  rotation, 0.0f);
        case '8':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Top, position, scale,
                                                  rotation, 0.0f);
        case '9':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::All, position, scale,
                                                  rotation, 0.0f);
        case 'A':
            return std::make_shared<Water>(position, scale, rotation, 0.0f);
        case 'B':
            return std::make_shared<Bush>(position, scale, rotation, 1.0f);
        case 'C':
            return std::make_shared<Ice>(position, scale, rotation, -1.0f);
        case 'D':
            return nullptr;
        case 'E':
            return std::make_shared<Eagle>(position, scale, rotation, 0.0f);
        default:
            std::cerr << "Can't find game object with description " << description << std::endl;
    }

    return nullptr;
}

Level::Level(const std::vector<std::string> &description) {
    if (description.empty()) {
        std::cerr << "Level description is empty" << std::endl;
    }

    width = description[0].length();
    height = description.size();

    playerRespawn1 = {BLOCK_SIZE * (width / 2 - 1), BLOCK_SIZE / 2};
    playerRespawn2 = {BLOCK_SIZE * (width / 2 + 3), BLOCK_SIZE / 2};

    gameObjects.reserve(width * height + 4);

    unsigned int currentBottomOffset = BLOCK_SIZE * (height - 1) + BLOCK_SIZE / 2;

    for (const std::string &currentRow: description) {
        unsigned int currentLeftOffset = BLOCK_SIZE;

        for (const char currentElement: currentRow) {
            switch (currentElement) {
                case 'K':
                    playerRespawn1 = {currentLeftOffset, currentBottomOffset};
                    break;
                case 'L':
                    playerRespawn2 = {currentLeftOffset, currentBottomOffset};
                    break;
                default:
                    gameObjects.emplace_back(
                            createFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset),
                                                  glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.0f));
                    break;
            }

            currentLeftOffset += BLOCK_SIZE;
        }

        currentBottomOffset -= BLOCK_SIZE;
    }

    gameObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0),
                                                      glm::vec2(width * BLOCK_SIZE, BLOCK_SIZE / 2), 0, 0));
    gameObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, height * BLOCK_SIZE + BLOCK_SIZE / 2),
                                                      glm::vec2(width * BLOCK_SIZE, BLOCK_SIZE / 2), 0, 0));
    gameObjects.emplace_back(std::make_shared<Border>(glm::vec2(0, 0),
                                                      glm::vec2(BLOCK_SIZE, (height + 1) * BLOCK_SIZE), 0, 0));
    gameObjects.emplace_back(std::make_shared<Border>(glm::vec2((width + 1) * BLOCK_SIZE, 0),
                                                      glm::vec2(2 * BLOCK_SIZE, (height + 1) * BLOCK_SIZE), 0, 0));
}

void Level::render() const {
    for (const auto &currentObject: gameObjects) {
        if (currentObject) {
            currentObject->render();
        }
    }
}

void Level::update(const double delta) {
    for (const auto &currentObject: gameObjects) {
        if (currentObject) {
            currentObject->update(delta);
        }
    }
}

size_t Level::get_width() const {
    return (width + 3) * BLOCK_SIZE;
}

size_t Level::get_height() const {
    return (height + 1) * BLOCK_SIZE;
}
