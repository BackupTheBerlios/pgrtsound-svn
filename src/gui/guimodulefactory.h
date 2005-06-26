#ifndef GUIMODULEFACTORY_H
#define GUIMODULEFACTORY_H

#include "../gui2/guimodules/guimodule.h"
#include "../gui2/guimodules/slidergui.h"
#include "../gui2/guimodules/sinosc2testgui.h"
#include "../gui2/guimodules/gaingui.h"

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
