#include "Level.h"

#include "GameObjects/BrickWall.h"
#include "GameObjects/ConcreteWall.h"
#include "GameObjects/Bush.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"

#include <cmath>
#include <iostream>
#include <algorithm>

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

    widthBlocks = description[0].length();
    heightBlocks = description.size();

    widthPixels = widthBlocks * BLOCK_SIZE;
    heightPixels = heightBlocks * BLOCK_SIZE;

    playerRespawn1 = {BLOCK_SIZE * (widthBlocks / 2 - 1), BLOCK_SIZE / 2};
    playerRespawn2 = {BLOCK_SIZE * (widthBlocks / 2 + 3), BLOCK_SIZE / 2};

    gameObjects.reserve(widthBlocks * heightBlocks + 4);

    unsigned int currentBottomOffset = BLOCK_SIZE * (heightBlocks - 1) + BLOCK_SIZE / 2;

    for (const std::string &currentRow: description) {
        unsigned int currentLeftOffset = BLOCK_SIZE;

        for (const char currentElement: currentRow) {
            switch (currentElement) {
                case 'K':
                    playerRespawn1 = {currentLeftOffset, currentBottomOffset};
                    gameObjects.emplace_back(nullptr);
                    break;
                case 'L':
                    playerRespawn2 = {currentLeftOffset, currentBottomOffset};
                    gameObjects.emplace_back(nullptr);
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
                                                      glm::vec2(widthPixels, BLOCK_SIZE / 2), 0, 0));
    gameObjects.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, heightPixels + BLOCK_SIZE / 2),
                                                      glm::vec2(widthPixels, BLOCK_SIZE / 2), 0, 0));
    gameObjects.emplace_back(std::make_shared<Border>(glm::vec2(0, 0),
                                                      glm::vec2(BLOCK_SIZE, (heightBlocks + 1) * BLOCK_SIZE), 0, 0));
    gameObjects.emplace_back(std::make_shared<Border>(glm::vec2((widthBlocks + 1) * BLOCK_SIZE, 0),
                                                      glm::vec2(2 * BLOCK_SIZE, (heightBlocks + 1) * BLOCK_SIZE), 0,
                                                      0));
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
    return (widthBlocks + 3) * BLOCK_SIZE;
}

size_t Level::get_height() const {
    return (heightBlocks + 1) * BLOCK_SIZE;
}

std::vector<std::shared_ptr<IGameObject>>
Level::get_objectsInArea(const glm::vec2 &bottomLeft, const glm::vec2 &topRight) const {
    std::vector<std::shared_ptr<IGameObject>> output;
    output.reserve(9);

    glm::vec2 convertedBottomLeft(std::clamp(bottomLeft.x - BLOCK_SIZE, 0.0f, static_cast<float>(widthPixels)),
                                  std::clamp(static_cast<float>(heightPixels) - bottomLeft.y +
                                             static_cast<float>(BLOCK_SIZE) / 2, 0.0f,
                                             static_cast<float>(heightPixels)));
    glm::vec2 convertedTopRight(std::clamp(topRight.x - BLOCK_SIZE, 0.0f, static_cast<float>(widthPixels)),
                                std::clamp(static_cast<float>(heightPixels) - topRight.y +
                                           static_cast<float>(BLOCK_SIZE) / 2, 0.0f, static_cast<float>(heightPixels)));

    size_t startX = std::floor(convertedBottomLeft.x / BLOCK_SIZE);
    size_t endX = std::ceil(convertedTopRight.x / BLOCK_SIZE);

    size_t startY = std::floor(convertedTopRight.y / BLOCK_SIZE);
    size_t endY = std::ceil(convertedBottomLeft.y / BLOCK_SIZE);

    for (size_t currentColumn = startX; currentColumn < endX; ++currentColumn) {
        for (size_t currentRow = startY; currentRow < endY; ++currentRow) {
            auto &currentObject = gameObjects[currentRow * widthBlocks + currentColumn];

            if (currentObject) {
                output.push_back(currentObject);
            }
        }
    }

    if (endX >= widthBlocks) {
        output.push_back(gameObjects[gameObjects.size() - 1]);
    }
    if (startX <= 1) {
        output.push_back(gameObjects[gameObjects.size() - 2]);
    }
    if (startY <= 1) {
        output.push_back(gameObjects[gameObjects.size() - 3]);
    }
    if (endY >= heightBlocks) {
        output.push_back(gameObjects[gameObjects.size() - 4]);
    }

    return output;
}
