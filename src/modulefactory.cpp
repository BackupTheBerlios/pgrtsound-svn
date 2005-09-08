#include "modulefactory.h"

#include "modules/sinosc2.h"
#include "modules/constant.h"
#include "modules/sum.h"
#include "modules/gain.h"

#include "modules/textfileout.h"
#include "modules/slider.h"
#include "modules/multiplication.h"
#include "modules/noise.h"
#include "modules/filter12db.h"
#include "modules/mgate.h"
#include "modules/mmonitor.h"
#include "modules/madsrlinear.h"

#include <dirent.h>

ModuleFactory::ModuleFactory() {
	RegisterModuleType( Constant::GetTypeStatic(), Constant::Create );
	RegisterModuleType( SinOsc2::GetTypeStatic(), SinOsc2::Create );
	RegisterModuleType( Sum::GetTypeStatic(), Sum::Create );
	RegisterModuleType( Slider::GetTypeStatic(), Slider::Create );
	RegisterModuleType( Gain::GetTypeStatic(), Gain::Create );
	RegisterModuleType( Multiplication::GetTypeStatic(), Multiplication::Create );
	RegisterModuleType( Filter12dB::GetTypeStatic(), Filter12dB::Create );
	RegisterModuleType( Noise::GetTypeStatic(), Noise::Create );
	RegisterModuleType( TextFileOut::GetTypeStatic(), TextFileOut::Create );
	RegisterModuleType( MGate::GetTypeStatic(), MGate::Create );
	RegisterModuleType( MMonitor::GetTypeStatic(), MMonitor::Create );
	RegisterModuleType( MADSRLinear::GetTypeStatic(), MADSRLinear::Create );

	if ( g_module_supported() == TRUE ) {
    	RegisterAllPlugins();
    }
}

ModuleFactory::~ModuleFactory() {
}

/**
 Tworzy modul wybranego typu.
 Po pomyslnym utworzeiu modulu zostaje zwrocony wskaznik od niego. Usuwaniem
 utoworznych modulow musi zajac sie programista.
 @param type Typ modulu.
*/
Module* ModuleFactory::CreateModule(string type) {

	CreateFuncPtr funcPtr = NULL;
	funcPtr = ( *type2CreateFuncMap.find(type) ).second;

	if( funcPtr != NULL) {
		// tworzymu modul danego typu
		return (*funcPtr)();
	}

    throw RTSError( "ModuleFactory::CreateModule - Nie ma modulu '" + type + "'" );
}

std::vector<string>& ModuleFactory::ListModuleTypes() {
	types.clear();
	for(map<string, CreateFuncPtr>::iterator it = type2CreateFuncMap.begin();
	    it != type2CreateFuncMap.end(); ++it) {
			types.push_back( it->first );
		}
	return types;
}

/**
 Zarejstroawnie typu modulu.
 Aby modul stal sie dostepny w systemie, konieczne jest zaresjtrowanie jego
 typu oraz zdefiniowanie funkcji ktora zajmuje sie tworzeniem obiektow tego typu.
 @param type Typ modulu do zarejstrowania - musi byc unikatowy w calym systemie
 @param funcPtr wskazink do funkcji tworzacej moduly rejestrowanego typu
*/
void ModuleFactory::RegisterModuleType(string type, CreateFuncPtr funcPtr) {
	type2CreateFuncMap.insert( make_pair(type, funcPtr) );
	TRACE( "ModuleFactory::RegisterModuleType - Zarejestrowany typ '%s'\n", type.c_str() );
}

void ModuleFactory::RegisterAllPlugins() {
    TRACE("ModuleFactory::RegisterAllPlugins - Start\n");
    DIR* dir;
    struct dirent* entry;
    dir = opendir("./plugins/");
    while ((entry = readdir(dir)) != NULL) {  
        TRACE( "ModuleFactory::RegisterAllPlugins - Wczytuje: %s\n", entry->d_name );
        RegisterPlugin (entry->d_name);
    }
    TRACE("ModuleFactory::RegisterAllPlugins - Koniec\n" );
}

void ModuleFactory::RegisterPlugin(string filename) {
    typedef string (* GetTypeFunc) ();
    typedef Module *(* CreateFunc) ();   
    GetTypeFunc  GetType;
    CreateFunc   Create;
   // GError       **error;
    
    filename = "./plugins/" + filename;
    
    GModule*  gm = g_module_open(filename.c_str(), G_MODULE_BIND_MASK);
    if (gm!=NULL) {
        if (g_module_symbol (gm, "GetType", (gpointer *)&GetType)==TRUE){                
            if (g_module_symbol (gm, "Create", (gpointer *)&Create)==TRUE){
                RegisterModuleType(GetType(), Create);
            } else {
                TRACE( "ModuleFactory::RegisterPlugin - g_module_error () w pliku %s\n",
					filename.c_str() );
            }
        } else {
            TRACE( "ModuleFactory::RegisterPlugin - g_module_error() w pliku %s\n",
				filename.c_str() );
        }                        
    }
}



