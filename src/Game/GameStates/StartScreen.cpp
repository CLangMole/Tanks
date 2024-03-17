#include "StartScreen.h"
#include "../../Resources/ResourceManager.h"

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

StartScreen::StartScreen(const std::vector<std::string> &description) {
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
}

void StartScreen::update(double delta) {

}

unsigned int StartScreen::get_width() const {
    return STARTSCREEN_WIDTH;
}

unsigned int StartScreen::get_height() const {
    return STARTSCREEN_HEIGHT;
}
