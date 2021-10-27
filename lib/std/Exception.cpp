#include "lib/std/Exception.hpp"

namespace lib
{
    Exception::Exception(std::string msg):
        _msg("Error: " + msg)
    {}

    const char *Exception::what() const throw()
    {
        return this->_msg.c_str();
    }

} // namespace lib
