#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <map>

#include "modules/module.h"
#include "modules/sinosc2.h"
#include "modules/constant.h"
#include "modules/sum.h"
#include "modules/gain.h"
#include "modules/audioportout.h"
#include "modules/audioportin.h"
#include "modules/textfileout.h"
#include "modules/slider.h"
#include "modules/multiplication.h"
#include "modules/noise.h"
#include "modules/filter12db.h"

#include <gmodule.h>

using namespace std;

// wskanzik do statycznych metod Module::Create()
typedef Module* (*CreateFuncPtr)();

/**
 * Docelowo klasa ma sie zjamowac ladowaniem wtyczek i ich tworzeniem.
 */
class ModuleFactory {
	/* TODO (#1#): Trzeba przmyslec te klase. musi dzialac dla 
                dynamicznie ladowanych wtyczek... */
	public:
		ModuleFactory();
		~ModuleFactory();
		void RegisterPlugin(string filename);
		void RegisterModuleType(string type, CreateFuncPtr funcPtr);
		void RegisterAllPlugins();
        Module* CreateModule(string type);
		
		
	private:
		map<string, CreateFuncPtr> type2CreateFuncMap;
};

#endif // MODULEFACTORY_H
