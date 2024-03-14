#include "SpriteAnimator.h"

using namespace RenderEngine;

SpriteAnimator::SpriteAnimator(std::shared_ptr<RenderEngine::Sprite> sprite)
        : sprite(std::move(sprite)), currentFrame(0), currentFrameDuration(this->sprite->get_frameDuration(0)),
          currentAnimationTime(0), totalDuration(0) {
    for (size_t frame = 0; frame < this->sprite->get_framesCount(); ++frame) {
        totalDuration += this->sprite->get_frameDuration(frame);
    }
}

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

void SpriteAnimator::reset() {
    currentFrame = 0;
    currentFrameDuration = sprite->get_frameDuration(0);
    currentAnimationTime = 0;
}
