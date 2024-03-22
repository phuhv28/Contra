#include "Timer.h"

Timer::Timer()
{
    isStarted = false;
}

Timer::~Timer() {}

void Timer::start()
{
    isStarted = true;
    startTick = CLOCK_NOW();
}

void Timer::stop()
{
    isStarted = false;
}

ElapsedTime Timer::getTick()
{
    return CLOCK_NOW() - startTick; 
}
