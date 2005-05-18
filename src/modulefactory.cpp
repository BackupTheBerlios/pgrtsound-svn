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
	
//	if (type == "SinOsc") {
//		return new SinOsc;
//	}
//
	if (type == "SinOsc2") {
		return new SinOsc2;
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

    throw RTSError("ModuleFactory::CreateModule(): Nie ma modulu '" + type + "'");
}
