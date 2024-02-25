#pragma once

#include "Sprite.h"
#include <memory>

namespace RenderEngine
{
    class SpriteAnimator
    {
    public:
        explicit SpriteAnimator(std::shared_ptr<RenderEngine::Sprite> sprite);

        [[nodiscard]] size_t getCurrentFrame() const {
            return currentFrame;
        }

        void update(double delta);

    private:
        std::shared_ptr<RenderEngine::Sprite> sprite;
        size_t currentFrame;
        double currentFrameDuration;
        double currentAnimationTime;
    };
}