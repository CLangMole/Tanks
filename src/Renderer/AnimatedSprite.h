#pragma once

#include "Sprite.h"
#include <vector>

namespace Renderer
{
    class AnimatedSprite : public Sprite
    {
    public:
        AnimatedSprite(std::shared_ptr<Texture2D> texture2D, std::string initialSubTexture2D,
               std::shared_ptr<ShaderProgram> shader,
               const glm::vec2 &position = glm::vec2(0.0f), const glm::vec2 &scale = glm::vec2(1.0f),
               const float rotation = 0.0f);

        void render() const override;
        void update(const uint64_t delta);

        void add_state(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration);
        void set_state(const std::string &newState);
    private:
        std::map<std::string, std::vector<std::pair<std::string, uint64_t>>> statesMap;

        size_t currentFrame = 0;
        uint64_t currentAnimationTime = 0;

        std::map<std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator currentAnimationDurations;

        mutable bool isDirty = false;
    };
}
