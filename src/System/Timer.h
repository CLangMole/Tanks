#pragma once

#include <functional>

class Timer
{
public:
    Timer();

    void start(double duration);

    void update(double delta);

    void on_complete(const std::function<void()> &function);

private:
    std::function<void()> callback;
    double timeLeft;
    bool isActive;
};