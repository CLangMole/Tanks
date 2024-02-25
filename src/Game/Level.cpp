#include "Level.h"
#include "GameObjects/BrickWall.h"
#include <iostream>

const unsigned int BLOCK_SIZE = 16;

std::shared_ptr<IGameObject> createFromDescription(const char description,
                                                   const glm::vec2 &position, const glm::vec2 &scale,
                                                   const float rotation) {
    switch (description) {
        case '0':
            return std::make_shared<BrickWall>(BrickWall::WallType::Right, position, scale, rotation);
        case '1':
            return std::make_shared<BrickWall>(BrickWall::WallType::Bottom, position, scale,
                                               rotation);
        case '2':
            return std::make_shared<BrickWall>(BrickWall::WallType::Left, position, scale,
                                               rotation);
        case '3':
            return std::make_shared<BrickWall>(BrickWall::WallType::Top, position, scale, rotation);
        case '4':
            return std::make_shared<BrickWall>(BrickWall::WallType::All, position, scale, rotation);

        case 'G':
            return std::make_shared<BrickWall>(BrickWall::WallType::BottomLeft, position, scale,
                                               rotation);
        case 'H':
            return std::make_shared<BrickWall>(BrickWall::WallType::BottomRight, position, scale,
                                               rotation);
        case 'I':
            return std::make_shared<BrickWall>(BrickWall::WallType::TopLeft, position, scale,
                                               rotation);
        case 'J':
            return std::make_shared<BrickWall>(BrickWall::WallType::TopRight, position, scale,
                                               rotation);
        case 'D':
            return nullptr;
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

    gameObjects.reserve(width * height + 4);

    unsigned int currentBottomOffset = BLOCK_SIZE * (height - 1);

    for (const std::string &currentRow: description) {
        unsigned int currentLeftOffset = 0;

        for (const char currentElement: currentRow) {
            gameObjects.emplace_back(
                    createFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset),
                                          glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.0f));

            currentLeftOffset += BLOCK_SIZE;
        }

        currentBottomOffset -= BLOCK_SIZE;
    }
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
