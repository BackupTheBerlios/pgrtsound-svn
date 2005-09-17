#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <stdarg.h>
#include <stdexcept>
#include <string>

#define PR(x) std::cout << #x ": " << x << std::endl;

void TRACE( const char* format, ... );

class RTSError : public std::runtime_error {
    public:
        RTSError (const std::string& msg = "") : std::runtime_error(msg) {}
};

#endif // DEBUG_H
