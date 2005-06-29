#include "modulefactory.h"
#include <dirent.h>

ModuleFactory::ModuleFactory() {
	RegisterModuleType("constant", Constant::Create);
	RegisterModuleType("sinosc2", SinOsc2::Create);
	//RegisterModuleType("sumator", Sum::Create);
	RegisterModuleType("slider", Slider::Create);
	RegisterModuleType("gain", Gain::Create);
	RegisterModuleType("multiplication", Multiplication::Create);
	RegisterModuleType("filter12db", Filter12dB::Create);
	RegisterModuleType("noise", Noise::Create);
	RegisterModuleType("textfileout", TextFileOut::Create);
	
	if (g_module_supported() == TRUE) {
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

    throw RTSError("ModuleFactory::CreateModule(): Nie ma modulu '" + type + "'");
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
	TRACE3("ModuleFactory::RegisterModuleType()", "Zarejestrowany typ '", type, "'");
}



void ModuleFactory::RegisterAllPlugins() {
    TRACE("ModuleFactory::RegisterAllPlugins()", "start");
    DIR* dir;
    struct dirent* entry;
    dir = opendir(".\\plugins\\");    
    while ((entry = readdir(dir)) != NULL) {  
        TRACE3("ModuleFactory::RegisterAllPlugins()", "wczytuje: ", entry->d_name ,"");
        RegisterPlugin (entry->d_name);
    }
    TRACE("ModuleFactory::RegisterAllPlugins()", "end");
}



void ModuleFactory::RegisterPlugin(string filename) {
    typedef string (* GetTypeFunc) ();
    typedef Module *(* CreateFunc) ();   
    GetTypeFunc  GetType;
    CreateFunc   Create;
    GError       **error;
    
    filename = ".\\plugins\\"+filename;
    
    GModule*  gm = g_module_open(filename.c_str(), G_MODULE_BIND_MASK);
    if (gm!=NULL) {
        if (g_module_symbol (gm, "GetType", (gpointer *)&GetType)==TRUE){                
            if (g_module_symbol (gm, "Create", (gpointer *)&Create)==TRUE){
                RegisterModuleType(GetType(), Create);
            } else {
                TRACE3("ModuleFactory::RegisterPlugin(string filename)","g_module_error ()"," w plku ", filename);
            }
        } else {
            TRACE3("ModuleFactory::RegisterPlugin(string filename)","g_module_error ()"," w plku ", filename);
        }                        
    }

    
}



