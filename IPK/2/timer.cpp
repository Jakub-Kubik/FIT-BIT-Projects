
/**
 *  TIMER.CPP AND TIMER.H ARE USED FROM INTERNET
 *  RESOURCES: https://gist.github.com/mcleary/b0bf4fa88830ff7c882d */

#include <thread>

#include "timer.h"


void Timer::start() {

    m_StartTime = std::chrono::system_clock::now();
    m_bRunning = true;
}


void Timer::stop() {

    m_EndTime = std::chrono::system_clock::now();
    m_bRunning = false;
}


long double Timer::elapsedMicroseconds() {

    std::chrono::time_point<std::chrono::system_clock> endTime;
    
    if ( m_bRunning ) {
        endTime = std::chrono::system_clock::now();
    }
    else {
        endTime = m_EndTime;
    }
    
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - m_StartTime).count();
}


long double Timer::elapsedMilliseconds() {

    return elapsedMicroseconds() / 1000.0;
}


long double Timer::elapsedSeconds() {

    return elapsedMilliseconds() / 1000.0;
}


void Timer::sleep(long int time)  {
    
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}
