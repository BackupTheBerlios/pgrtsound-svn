#include "modulefactory.h"

ModuleFactory::ModuleFactory() {
}

ModuleFactory::~ModuleFactory() {
}

/**
 * Towrzy modul wybranego typu i zwraca do niego wskaznik
 */
Module* ModuleFactory::createModule(string type) {
	if (type == "Constant") {
		return new Constant;
	}
	
	if (type == "SinOsc") {
		return new SinOsc;
	}
	
	if (type == "Sumator") {
		return new Sum;
	}
	
	if (type == "Gain") {
		return new Gain;
	}
    
    
    //exit (1);
	// throw bo nie ma modulu?

}
