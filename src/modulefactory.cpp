#include "modulefactory.h"

ModuleFactory::ModuleFactory() {
}

ModuleFactory::~ModuleFactory() {
}

/**
 * Towrzy modul wybranego typu i zwraca do niego wskaznik
 */
Module* ModuleFactory::CreateModule(string type) {
	if (type == "constant") {
		return new Constant;
	}
	
//	if (type == "SinOsc") {
//		return new SinOsc;
//	}
//
	if (type == "sinosc2") {
		return new SinOsc2;
	}

	if (type == "sumator") {
		return new Sum;
	}

	if (type == "gain") {
		return new Gain;
	}
	
	if (type == "slider") {
		return new Slider;
	}
	
	if (type == "multiplication") {
		return new Multiplication;
	}

	if (type == "textfileout") {
		return new TextFileOut;
	}

    throw RTSError("ModuleFactory::CreateModule(): Nie ma modulu '" + type + "'");
}
