#include "timer.h"
#include <Arduino.h>

namespace utils
{
    timer::timer()
        : m_previous(0), m_interval(0)
    {
    }
    timer::timer(uint32_t interval)
        : m_previous(0), m_interval(interval)
    {
    }
    timer::timer(const timer &other)
        : m_previous(other.m_previous), m_interval(other.m_interval)
    {
    }
    timer &timer::operator=(const timer &other)
    {
        if (this != &other)
        {
            m_previous = other.m_previous;
            m_interval = other.m_interval;
        }
        return *this;
    }

    void timer::start(uint32_t interval)
    {
        m_interval = (interval == 0) ? m_interval : interval;
        reset();
    }
    void timer::reset()
    {
        m_previous = millis();
    }
    bool timer::check() 
    {
        const uint32_t current = millis();

        if (current - m_previous >= m_interval)
        {
            m_previous = current;
            return true;
        }
        return false;
    }
}