#pragma once

#include <vector>
#include <string>
#include "IGameState.h"
#include "../../Renderer/Sprite.h"
#include "../../Renderer/SpriteAnimator.h"

class Game;

class StartScreen : public IGameState
{
public:
    static constexpr unsigned int BLOCK_SIZE = 8;

    static constexpr unsigned int STARTSCREEN_WIDTH = 15 * BLOCK_SIZE * 2;
    static constexpr unsigned int STARTSCREEN_HEIGHT = 14 * BLOCK_SIZE * 2;

    static constexpr unsigned int MENU_WIDTH = 128;
    static constexpr unsigned int MENU_HEIGHT = 64;

    static constexpr unsigned int TANK_SIZE = BLOCK_SIZE * 2;

    StartScreen(const std::vector<std::string> &description, Game *game);

    void render() const override;

    void update(double delta) override;

    void handle_input(const std::array<bool, 349> &keys) override;

    [[nodiscard]] unsigned int get_width() const override;

    [[nodiscard]] unsigned int get_height() const override;

private:
    Game *game;
    bool keyReleased;
    int currentMenuSelection;

    std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2>> sprites;

    std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> menuSprite;
    std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2> tankSprite;

    RenderEngine::SpriteAnimator tankAnimator;
};
