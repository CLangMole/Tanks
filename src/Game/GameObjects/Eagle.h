#pragma once

#include <memory>
#include <array>
#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

class Eagle : public IGameObject
{
public:
    enum class EagleState : uint8_t
    {
        Alive = 0,
        Dead
    };

    Eagle(const glm::vec2 &position, const glm::vec2 &scale, float rotation);

    void render() const override;

    void update(double delta) override;

private:
    std::array<std::shared_ptr<RenderEngine::Sprite>, 2> sprites;
    EagleState currentState;
};
