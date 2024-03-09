#include "Timer.h"

Timer::Timer() : isActive(false), timeLeft(0) {

}

void Timer::start(double duration) {
    timeLeft = duration;
    isActive = true;
}

void Timer::update(double delta) {
    if (isActive) {
        timeLeft -= delta;

        if (timeLeft <= 0) {
            isActive = false;
            callback();
        }
    }
}

void Timer::on_complete(const std::function<void()> &function) {
    callback = function;
}
