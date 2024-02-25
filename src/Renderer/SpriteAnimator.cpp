#include "SpriteAnimator.h"

using namespace RenderEngine;

SpriteAnimator::SpriteAnimator(std::shared_ptr<RenderEngine::Sprite> sprite)
        : sprite(std::move(sprite)), currentFrame(0), currentFrameDuration(this->sprite->get_frameDuration(0)),
          currentAnimationTime(0) {}

void SpriteAnimator::update(const double delta) {
    currentAnimationTime += delta;
    while (currentAnimationTime >= currentFrameDuration) {
        currentAnimationTime -= currentFrameDuration;
        ++currentFrame;

        if (currentFrame == sprite->get_framesCount()) {
            currentFrame = 0;
        }

        currentFrameDuration = sprite->get_frameDuration(currentFrame);
    }
}
