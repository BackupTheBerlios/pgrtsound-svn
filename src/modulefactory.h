#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include "modules/module.h"
#include "modules/sinosc.h"
#include "modules/constant.h"
#include "modules/sum.h"
#include "modules/gain.h"
#include "modules/audioportout.h"
#include "modules/TextFileOut.h"

/**
 * Docelowo klasa ma sie zjamowac ladowaniem wtyczek i ich tworzeniem.
 */
class ModuleFactory {
	/* TODO (#1#): Trzeba przmyslec te klase. musi dzialac dla 
                dynamicznie ladowanych wtyczek... */
	public:
		ModuleFactory();
		~ModuleFactory();
		Module* createModule(string type);
};

#endif // MODULEFACTORY_H
