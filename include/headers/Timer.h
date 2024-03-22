#ifndef TIMER_H
#define TIMER_H

#include "Constants.h"

class Timer
{
public:
    Timer();
    ~Timer();
    void start();
    void stop();
    ElapsedTime getTick();
    bool getisStarted() { return isStarted; }
private:
    std::chrono::system_clock::time_point startTick;
    std::chrono::system_clock::time_point endTick;
    bool isStarted;
};


#endif