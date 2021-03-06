#ifndef GUIMODULEFACTORY_H
#define GUIMODULEFACTORY_H

#include "../debug.h"
//#include "guis/gui.h"
#include "guimodule.h"
//#include "guimodules/slidergui.h"
//#include "guimodules/sinosc2testgui.h"
#include "guis/ggain.h"
#include "guis/gconstant.h"
#include "guis/gslider.h"
#include "guis/ggate.h"
#include "guis/gmonitor.h"
#include "guis/gfilter.h"
#include "guis/gadsr.h"
#include "guis/gmonitorgraph.h"
#include "guis/gdelay.h"
#include "guis/gfeedbackdelay.h"
#include "guis/gvariabledelay.h"
//#include "guis/gfft.h"
#include <map>


/**
 Dodawanie interfjesow graficznych dla modulow.
*/
typedef Gui* (*CreateGuiFuncPtr)( Module* module );

class GuiModuleFactory {
	/* TODO (#1#): Trzeba przmyslec te klase. musi dzialac dla 
                dynamicznie ladowanych wtyczek... */
	public:
		GuiModuleFactory();
		~GuiModuleFactory();
		GuiModule* CreateGuiModule( Module* module );
		void RegisterGui( string type, CreateGuiFuncPtr funcPtr );
		void RegisterGuiPlugin( string filename );
		void RegisterAllGuiPlugins();

	protected:
		map<string, CreateGuiFuncPtr> type2GuiMap;
};

#endif // GUIMODULEFACTORY_H
