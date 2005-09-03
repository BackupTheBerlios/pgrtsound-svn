#ifndef GUIMODULEFACTORY_H
#define GUIMODULEFACTORY_H

#include "../debug.h"
#include "guimodules/guimodule.h"
#include "guimodules/slidergui.h"
#include "guimodules/sinosc2testgui.h"
#include "guimodules/gaingui.h"
#include "guimodules/gmconstant.h"

/**
	Dodawanie interfjesow graficznych dla modulow.
*/
class GuiModuleFactory {
	/* TODO (#1#): Trzeba przmyslec te klase. musi dzialac dla 
                dynamicznie ladowanych wtyczek... */
	public:
		GuiModuleFactory();
		~GuiModuleFactory();
		GuiModule* CreateGuiModule(Module* module);
};

#endif // GUIMODULEFACTORY_H
