#pragma once

#include <vector>
#include <memory>
#include "..//Game/GameObjects/IGameObject.h"

class Level
{
public:
    explicit Level(const std::vector<std::string> &description);

    void render() const;

    void update(double delta);

private:
    size_t width;
    size_t height;

    std::vector<std::shared_ptr<IGameObject>> gameObjects;
};
