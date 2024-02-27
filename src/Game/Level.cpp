#include "Level.h"

#include "GameObjects/BrickWall.h"
#include "GameObjects/ConcreteWall.h"
#include "GameObjects/Bush.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"

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
        case '5':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Right, position, scale, rotation);
        case '6':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Bottom, position, scale, rotation);
        case '7':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Left, position, scale, rotation);
        case '8':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::Top, position, scale, rotation);
        case '9':
            return std::make_shared<ConcreteWall>(ConcreteWall::WallType::All, position, scale, rotation);
        case 'A':
            return std::make_shared<Water>(position, scale, rotation);
        case 'B':
            return std::make_shared<Bush>(position, scale, rotation);
        case 'C':
            return std::make_shared<Ice>(position, scale, rotation);
        case 'D':
            return nullptr;
        case 'E':
            return std::make_shared<Eagle>(position, scale, rotation);
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
