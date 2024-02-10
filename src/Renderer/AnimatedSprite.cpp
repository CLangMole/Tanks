#include "AnimatedSprite.h"

using namespace Renderer;

AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture2D> texture2D,
                                         std::string initialSubTexture2D,
                                         std::shared_ptr<ShaderProgram> shader, const glm::vec2 &position,
                                         const glm::vec2 &scale, const float rotation)
        : Sprite(std::move(texture2D), std::move(initialSubTexture2D), std::move(shader), position, scale, rotation) {
    currentAnimationDurations = statesMap.end();
}

void AnimatedSprite::add_state(std::string state,
                                         std::vector<std::pair<std::string, uint64_t>> subTexturesDuration) {
    statesMap.emplace(std::move(state), std::move(subTexturesDuration));
}

void AnimatedSprite::set_state(const std::string& newState) {
    auto it = statesMap.find(newState);

    if (it == statesMap.end()){
        std::cout << "Can't find animation state " << newState << std::endl;
        return;
    }

    if (it != currentAnimationDurations){
        currentAnimationTime = 0;
        currentFrame = 0;
        currentAnimationDurations = it;
        isDirty = true;
    }
}

void AnimatedSprite::update(const uint64_t delta) {
    if (currentAnimationDurations != statesMap.end()){
        currentAnimationTime += delta;

        while (currentAnimationTime >= currentAnimationDurations->second[currentFrame].second){
            currentAnimationTime -= currentAnimationDurations->second[currentFrame].second;
            ++currentFrame;
            isDirty = true;

            if (currentFrame == currentAnimationDurations->second.size()){
                currentFrame = 0;
            }
        }
    }
}

void AnimatedSprite::render() const {
    if (isDirty){
        Texture2D::SubTexture2D subTexture = texture2D->get_subTexture(currentAnimationDurations->second[currentFrame].first);

        const GLfloat texturePosition[]{
                subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
                subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
                subTexture.rightTopUV.x, subTexture.rightTopUV.y,

                subTexture.rightTopUV.x, subTexture.rightTopUV.y,
                subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
                subTexture.leftBottomUV.x, subTexture.leftBottomUV.y
        };

        glBindBuffer(GL_ARRAY_BUFFER, texturePositionVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(texturePosition), &texturePosition);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        isDirty = false;
    }

    Sprite::render();
}