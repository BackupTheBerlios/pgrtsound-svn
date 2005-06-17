#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include "modules/module.h"
//#include "modules/sinosc.h"
#include "modules/sinosc2.h"
#include "modules/constant.h"
#include "modules/sum.h"
#include "modules/gain.h"
#include "modules/audioportout.h"
#include "modules/audioportin.h"
#include "modules/textfileout.h"
#include "modules/slider.h"
#include "modules/multiplication.h"
#include "modules/synthesis/noise.h"
#include "modules/filters/filter12db.h"

/**
 * Docelowo klasa ma sie zjamowac ladowaniem wtyczek i ich tworzeniem.
 */
class ModuleFactory {
	/* TODO (#1#): Trzeba przmyslec te klase. musi dzialac dla 
                dynamicznie ladowanych wtyczek... */
	public:
		ModuleFactory();
		~ModuleFactory();
		Module* CreateModule(string type);
};

#endif // MODULEFACTORY_H
