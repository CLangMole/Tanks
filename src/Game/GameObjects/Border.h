#pragma once

#include <memory>
#include <array>
#include "IGameObject.h"
#include "../../Renderer/Sprite.h"

class Border : public IGameObject
{
public:
    Border(const glm::vec2 &position, const glm::vec2 &scale, float rotation, float layer);

    void render() const override;

private:
    std::shared_ptr<RenderEngine::Sprite> sprite;
};
