#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#define PR(x) std::cout << #x ": " << x << std::endl;

#ifndef NDEBUG
	#define TRACE(kto, co) std::cout << kto << ": " << co << std::endl
	#define TRACE2(kto, co1, co2) std::cout << kto << ": " << co1 << co2 << std::endl
	#define TRACE3(kto, co1, co2, co3) std::cout << kto << ": " << co1 << co2 << co3 << std::endl
	#define TRACE4(kto, co1, co2, co3, co4) std::cout << kto << ": " << co1 << co2 << co3 << co4 << std::endl
	#define TRACE5(kto, co1, co2, co3, co4, co5) std::cout << kto << ": " << co1 << co2 << co3 << co4 << co5 << std::endl
	#define TRACE6(kto, co1, co2, co3, co4, co5, co6) std::cout << kto << ": " << co1 << co2 << co3 << co4 << co5 << co6 << std::endl
#else
	#define TRACE(kto, co)
	#define TRACE2(kto, co1, co2)
	#define TRACE3(kto, co1, co2, co3)
	#define TRACE4(kto, co1, co2, co3, co4)
	#define TRACE5(kto, co1, co2, co3, co4, co5)
#endif

#include <stdexcept>
#include <string>

class RTSError : public std::runtime_error {
    public:
        RTSError (const std::string& msg = "") : std::runtime_error(msg) {}
};

#endif // DEBUG_H
