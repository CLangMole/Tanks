#include "StartScreen.h"
#include "../Game.h"
#include "../../Resources/ResourceManager.h"

#include <GLFW/glfw3.h>
#include <array>

std::shared_ptr<RenderEngine::Sprite> get_spriteFromDescription(const char description) {
    switch (description) {
        case '0':
            return ResourceManager::get_sprite("brickWall_All");
        case '1':
            return ResourceManager::get_sprite("brickWall_TopLeft");
        case '2':
            return ResourceManager::get_sprite("brickWall_TopRight");
        case '3':
            return ResourceManager::get_sprite("brickWall_Top");
        case '4':
            return ResourceManager::get_sprite("brickWall_BottomLeft");
        case '5':
            return ResourceManager::get_sprite("brickWall_Left");
        case '6':
            return ResourceManager::get_sprite("brickWall_TopRight_BottomLeft");
        case '7':
            return ResourceManager::get_sprite("brickWall_Top_BottomLeft");
        case '8':
            return ResourceManager::get_sprite("brickWall_BottomRight");
        case '9':
            return ResourceManager::get_sprite("brickWall_TopLeft_BottomRight");
        case 'A':
            return ResourceManager::get_sprite("brickWall_Right");
        case 'B':
            return ResourceManager::get_sprite("brickWall_Top_BottomRight");
        case 'C':
            return ResourceManager::get_sprite("brickWall_Bottom");
        case 'D':
            return ResourceManager::get_sprite("brickWall_TopLeft_Bottom");
        case 'E':
            return ResourceManager::get_sprite("brickWall_TopRight_Bottom");
        case 'F':
            return nullptr;
        default:
            std::cerr << "Unknown Start Level description: " << description << std::endl;
    }

    return nullptr;
}

StartScreen::StartScreen(const std::vector<std::string> &description, Game *game)
        : game(game),
          keyReleased(true),
          currentMenuSelection(0),
          menuSprite(std::make_pair(ResourceManager::get_sprite("menu"),
                                    glm::vec2(11 * BLOCK_SIZE,
                                              STARTSCREEN_HEIGHT -
                                              description.size() * BLOCK_SIZE - MENU_HEIGHT -
                                              5 * BLOCK_SIZE))),
          tankSprite(ResourceManager::get_sprite("player2_green_tank_type1_sprite_right"),
                     glm::vec2(8 * BLOCK_SIZE,
                               menuSprite.second.y + 6 * BLOCK_SIZE -
                               static_cast<float>(currentMenuSelection) * 2 * BLOCK_SIZE)),
          tankAnimator(tankSprite.first) {
    if (description.empty()) {
        std::cerr << "Empty start screen description!" << std::endl;
    }

    auto leftOffsetPixels = 4 * BLOCK_SIZE;
    auto bottomOffset = STARTSCREEN_HEIGHT - 4 * BLOCK_SIZE;

    unsigned int currentBottomOffset = bottomOffset;
    for (const std::string &currentRow: description) {
        unsigned int currentLeftOffset = leftOffsetPixels;

        for (const char currentElement: currentRow) {
            sprites.emplace_back(std::make_pair(get_spriteFromDescription(currentElement),
                                                glm::vec2(currentLeftOffset, currentBottomOffset)));
            currentLeftOffset += BLOCK_SIZE;
        }

        currentBottomOffset -= BLOCK_SIZE;
    }
}

void StartScreen::render() const {
    for (const auto &currentSprite: sprites) {
        if (currentSprite.first) {
            currentSprite.first->render(currentSprite.second, glm::vec2(BLOCK_SIZE), 0.0f);
        }
    }

    menuSprite.first->render(menuSprite.second, glm::vec2(MENU_WIDTH, MENU_HEIGHT), 0.0f);
    tankSprite.first->render(
            glm::vec2(tankSprite.second.x,
                      tankSprite.second.y - static_cast<float>(currentMenuSelection) * 2 * BLOCK_SIZE),
            glm::vec2(TANK_SIZE), 0.0f, 0.0f, tankAnimator.get_currentFrame());
}

void StartScreen::update(double delta) {
    tankAnimator.update(delta);
}

void StartScreen::handle_input(const std::array<bool, 349> &keys) {
    if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S]) {
        keyReleased = true;
    }

    if (keyReleased) {
        if (keys[GLFW_KEY_W]) {
            keyReleased = false;
            --currentMenuSelection;

            if (currentMenuSelection < 0) {
                currentMenuSelection = 2;
            }
        } else if (keys[GLFW_KEY_S]) {
            keyReleased = false;
            ++currentMenuSelection;

            if (currentMenuSelection > 2) {
                currentMenuSelection = 0;
            }
        }
    }
    if (keys[GLFW_KEY_ENTER]) {
        switch (currentMenuSelection) {
            case 0:
                game->start_level(0);
                break;
            default:
                break;
        }
    }
}

unsigned int StartScreen::get_width() const {
    return STARTSCREEN_WIDTH;
}

unsigned int StartScreen::get_height() const {
    return STARTSCREEN_HEIGHT;
}
