#include "modulefactory.h"

ModuleFactory::ModuleFactory() {
}

ModuleFactory::~ModuleFactory() {
}

/**
 * Towrzy modul wybranego typu i zwraca do niego wskaznik
 */
Module* ModuleFactory::CreateModule(string type) {
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
	
	if (type == "TextFileOut") {
		return new TextFileOut;
	}

    //exit (1);
	// throw bo nie ma modulu?
}
