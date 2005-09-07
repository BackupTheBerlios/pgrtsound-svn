#include "guimodulefactory.h"

GuiModuleFactory::GuiModuleFactory() {
	RegisterGui( GConstant::GetTypeStatic(), GConstant::Create );
	RegisterGui( GSlider::GetTypeStatic(), GSlider::Create );
	RegisterGui( GGate::GetTypeStatic(), GGate::Create );
	RegisterGui( GMonitor::GetTypeStatic(), GMonitor::Create );
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
GuiModule* GuiModuleFactory::CreateGuiModule( Module* module ) {
	GuiModule* guiMod = new GuiModule( module );
	CreateGuiFuncPtr funcPtr = NULL;

	if( type2GuiMap.count( module->GetType() ) == 1 ) {
		// tworzymu Gui
		funcPtr = ( *type2GuiMap.find( module->GetType() ) ).second;
		Gui* gui = (*funcPtr)( module );
		guiMod->SetGui( gui );
	}

	return guiMod;
}

void GuiModuleFactory::RegisterGui( string type, CreateGuiFuncPtr funcPtr ) {
	type2GuiMap.insert( make_pair(type, funcPtr) );
	TRACE( "GuiModuleFactory::RegisterGui - Zarejestrowany GUI dla typu '%s'\n",
		type.c_str() );
}
