#include <sstream>


template<typename T> std::string IntToString(const T& x) {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}
