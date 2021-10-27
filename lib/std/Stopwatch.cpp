#include "lib/std/Stopwatch.hpp"

namespace lib
{
    void Stopwatch::start(void)
    {
        this->_is_running = true;
        this->_start = std::chrono::system_clock::now();
    }

    void Stopwatch::stop(void)
    {
        this->_is_running = false;
        this->_end = std::chrono::system_clock::now();
    }

    void Stopwatch::restart(void)
    {
        this->start();
    }

    bool Stopwatch::is_running(void) const
    {
        return this->_is_running;
    }

    // Gets elapsed time as seconds
    double Stopwatch::getElapsedTime(void) const
    {
        if (this->_is_running) {
            return (std::chrono::duration<double> (std::chrono::system_clock::now() - _start)).count();
        } else {
            return (std::chrono::duration<double> (_end - _start)).count();
        }
    }
} // namespace arcade
