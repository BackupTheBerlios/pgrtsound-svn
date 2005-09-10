#include "mwavplayer.h"

#include <windows.h>
#include <string>

using std::string;

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */

// modul
extern "C" __declspec(dllexport) string __stdcall GetModuleType();
extern "C" __declspec(dllexport) Module * __stdcall CreateModule();

string GetModuleType() {
	return MWavPlayer::GetTypeStatic();
}

Module* CreateModule() {
	return MWavPlayer::Create();
}

// gui
#include "gwavplayer.h"

extern "C" __declspec(dllexport) string __stdcall GetGuiType();
extern "C" __declspec(dllexport) Gui* __stdcall CreateGui( Module* mod );

string GetGuiType() {
	return GWavPlayer::GetTypeStatic();
}

Gui* CreateGui( Module* mod ) {
	return GWavPlayer::Create( mod );
}



