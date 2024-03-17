#pragma once

#include <vector>
#include <string>
#include "IGameState.h"
#include "../../Renderer/Sprite.h"

class StartScreen : public IGameState
{
public:
    static constexpr unsigned int BLOCK_SIZE = 8;
    static constexpr unsigned int STARTSCREEN_WIDTH = 15 * BLOCK_SIZE * 2;
    static constexpr unsigned int STARTSCREEN_HEIGHT = 14 * BLOCK_SIZE * 2;

    explicit StartScreen(const std::vector<std::string> &description);

    void render() const override;

    void update(double delta) override;

    [[nodiscard]] unsigned int get_width() const override;

    [[nodiscard]] unsigned int get_height() const override;

private:
    std::vector<std::pair<std::shared_ptr<RenderEngine::Sprite>, glm::vec2>> sprites;
};
