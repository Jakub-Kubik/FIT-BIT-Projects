
/**
 *  TIMER.CPP AND TIMER.H ARE USED FROM INTERNET
 *  RESOURCES: https://gist.github.com/mcleary/b0bf4fa88830ff7c882d */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
    void start();   
    void stop();   
    long double elapsedMicroseconds();
    long double elapsedMilliseconds();
    long double elapsedSeconds();
    void sleep(long int time);

private:
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    bool                                               m_bRunning = false;
};

#endif