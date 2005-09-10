#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include "modules/module.h"
#include "modules/audioportout.h"
#include "modules/audioportin.h"

#include <gmodule.h>

#include <map>
#include <vector>

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
		void RegisterModuleType( string type, CreateFuncPtr funcPtr );
        Module* CreateModule(string type);
        std::vector<string>& ListModuleTypes();
		void RegisterPlugin(string filename);
		void RegisterAllPlugins();

	private:
		map<string, CreateFuncPtr> type2CreateFuncMap;
		std::vector<string> types;
};

#endif // MODULEFACTORY_H
