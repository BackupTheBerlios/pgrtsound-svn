#include "modulefactory.h"
//#include "ltdl.h" 

ModuleFactory::ModuleFactory() {
  /*  LTDL_SET_PRELOADED_SYMBOLS();
    if (lt_dlinit() != 0) {
        throw RTSError("ModuleFactory::ModuleFactory(): LibTool - '");// + string(lt_dlerror()) + "'");
    } 
  */
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

	if (type == "audioportin") {
		return new AudioPortIn;
	}
	
	if (type == "audioportout") {
		return new AudioPortOut;
	}

    throw RTSError("ModuleFactory::CreateModule(): Nie ma modulu '" + type + "'");
}
