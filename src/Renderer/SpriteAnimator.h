#pragma once

#include "Sprite.h"
#include <memory>

namespace RenderEngine
{
    class SpriteAnimator
    {
    public:
        explicit SpriteAnimator(std::shared_ptr<RenderEngine::Sprite> sprite);

        void update(double delta);

        void reset();

        [[nodiscard]] size_t get_currentFrame() const {
            return currentFrame;
        }

        [[nodiscard]] double get_totalDuration() const {
            return totalDuration;
        }

    private:
        std::shared_ptr<RenderEngine::Sprite> sprite;
        size_t currentFrame;
        double currentFrameDuration;
        double currentAnimationTime;
        double totalDuration;
    };
}