#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

using namespace std;

#define PR(x) cout << #x ": " << x << endl;

#ifndef NDEBUG
	#define TRACE(kto, co) cout << kto << ": " << co << endl
	#define TRACE2(kto, co1, co2) cout << kto << ": " << co1 << co2 << endl
	#define TRACE3(kto, co1, co2, co3) cout << kto << ": " << co1 << co2 << co3 << endl
	#define TRACE4(kto, co1, co2, co3, co4) cout << kto << ": " << co1 << co2 << co3 << co4 << endl
	#define TRACE5(kto, co1, co2, co3, co4, co5) cout << kto << ": " << co1 << co2 << co3 << co4 << co5 << endl
	#define TRACE6(kto, co1, co2, co3, co4, co5, co6) cout << kto << ": " << co1 << co2 << co3 << co4 << co5 << co6 << endl
#else
	#define TRACE(kto, co)
	#define TRACE2(kto, co1, co2)
	#define TRACE3(kto, co1, co2, co3)
	#define TRACE4(kto, co1, co2, co3, co4)
	#define TRACE5(kto, co1, co2, co3, co4, co5)
#endif

#include <stdexcept>
#include <string>

class RTSError : public runtime_error {
    public:
        RTSError (const string& msg = "") : runtime_error(msg) {}
};

#endif // DEBUG_H
