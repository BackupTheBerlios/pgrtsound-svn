#include <windows.h>
#include <string>

#include "../../module.h"
#include "../../sum.h"


using std::string;

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

extern "C" __declspec(dllexport) string __stdcall GetType();
extern "C" __declspec(dllexport) Module * __stdcall Create();

string GetType() {
    return "sumator";   
}

Module * Create() {
    return Sum::Create();  
}




