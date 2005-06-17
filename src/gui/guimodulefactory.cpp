#include "guimodulefactory.h"

GuiModuleFactory::GuiModuleFactory() {
}

GuiModuleFactory::~GuiModuleFactory() {
}

/**
 	Tworzy obiekt modulu GUI.
	Jesli GUI dla modulu podanego typu istnieje zostanie ono utworzone a
	fnckja zwroci do niego wskaznik. Jesli specjalne GUI dla modulu podanego
	typu nie jest w systemie zdefiniowane zwracany jest zwykly obiekt GuiModule.

	@param type Typ modulu dla ktorego GUI chcemy utworzyc
 */
GuiModule* GuiModuleFactory::CreateGuiModule(Module* module) {
    //TRACE2("GuiModuleFactory::CreateGuiModule()", "Tworze GUI dla typu ", module->GetType());

	string type = module->GetType();

	if (type == "slider") {
		return new SliderGui(module);
	}
	
	if (type == "sinosc2") {
		return new SinOsc2TestGui(module);
	}
	
	if (type == "gain") {
		return new GainGui(module);
	}

	// nie ma specjalistycznego GuiModule zwracamy zwykly
	return new GuiModule(module);
}
