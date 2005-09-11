#include "guimodulefactory.h"

#include <dirent.h>

GuiModuleFactory::GuiModuleFactory() {
	RegisterGui( GConstant::GetTypeStatic(), GConstant::Create );
	RegisterGui( GSlider::GetTypeStatic(), GSlider::Create );
	RegisterGui( GGate::GetTypeStatic(), GGate::Create );
	RegisterGui( GMonitor::GetTypeStatic(), GMonitor::Create );
	RegisterGui( GFilter::GetTypeStatic(), GFilter::Create );
	RegisterGui( GGain::GetTypeStatic(), GGain::Create );
	RegisterGui( GADSR::GetTypeStatic(), GADSR::Create );
	RegisterGui( GSinLFO::GetTypeStatic(), GSinLFO::Create );
	RegisterGui( GMonitorGraph::GetTypeStatic(), GMonitorGraph::Create );
	RegisterGui( GDelay::GetTypeStatic(), GDelay::Create );
	if ( g_module_supported() == TRUE ) {
    	RegisterAllGuiPlugins();
    }
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

void GuiModuleFactory::RegisterAllGuiPlugins() {
    TRACE("GuiModuleFactory::RegisterAllGuiPlugins - Start\n");
    DIR* dir;
    struct dirent* entry;
    dir = opendir("./plugins/");
    while ((entry = readdir(dir)) != NULL) {
        TRACE( "GuiModuleFactory::RegisterAllGuiPlugins - Wczytuje: %s\n",
			entry->d_name );
        RegisterGuiPlugin( entry->d_name );
    }
    TRACE("GuiModuleFactory::RegisterAllGuiPlugins - Koniec\n" );
}

void GuiModuleFactory::RegisterGuiPlugin(string filename) {
    typedef string (* GetTypeFunc)();
    typedef Gui* (* CreateFunc)( Module* mod );
    GetTypeFunc  GetType;
    CreateFunc   Create;
   // GError       **error;

    filename = "./plugins/" + filename;

    GModule*  gm = g_module_open( filename.c_str(), G_MODULE_BIND_MASK );
    if( gm!=NULL ) {
		if( g_module_symbol( gm, "GetGuiType", (gpointer *)&GetType ) == TRUE ) {
            if( g_module_symbol( gm, "CreateGui", (gpointer *)&Create ) == TRUE ) {
                RegisterGui( GetType(), Create );
            }
			else {
				TRACE( "GuiModuleFactory::RegisterGuiPlugin - Nie znaleziono "
					"CreateGui() w %s\n", filename.c_str() );
            }
        }
		else {
            TRACE( "MGuiModuleFactory::RegisterGuiPlugin - Nie znaleziono CreateGui() w pliku %s\n",
				filename.c_str() );
        }
    }
}
