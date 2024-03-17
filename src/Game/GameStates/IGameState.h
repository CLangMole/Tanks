#pragma once

class IGameState
{
public:
    virtual ~IGameState() = default;

    virtual void render() const = 0;

    virtual void update(double delta) = 0;

    [[nodiscard]] virtual unsigned int get_width() const = 0;

    [[nodiscard]] virtual unsigned int get_height() const = 0;

};
