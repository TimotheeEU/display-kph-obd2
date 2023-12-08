#ifndef REV_INDICATOR_TIME_H
#define REV_INDICATOR_TIME_H

#include <stdint.h>

namespace utils
{
    class timer
    {
    public:
        timer();
        timer(uint32_t interval);
        ~timer() = default;

        timer(const timer &other);
        timer &operator=(const timer &other);

    public:
        void start(uint32_t interval = 0);
        void reset();

        bool check();

    private:
        uint32_t m_previous;
        uint32_t m_interval;
    };
}

#endif //REV_INDICATOR_TIME_H