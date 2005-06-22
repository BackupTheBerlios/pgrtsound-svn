#include "modulefactory.h"

ModuleFactory::ModuleFactory() {
	RegisterModuleType("constant", Constant::Create);
	RegisterModuleType("sinosc2", SinOsc2::Create);
	RegisterModuleType("sumator", Sum::Create);
	RegisterModuleType("slider", Slider::Create);
	RegisterModuleType("gain", Gain::Create);
	RegisterModuleType("multiplication", Multiplication::Create);
	RegisterModuleType("filter12db", Filter12dB::Create);
	RegisterModuleType("noise", Noise::Create);
	RegisterModuleType("textfileout", TextFileOut::Create);
}

ModuleFactory::~ModuleFactory() {
}

/**
 * Towrzy modul wybranego typu i zwraca do niego wskaznik
 */
Module* ModuleFactory::CreateModule(string type) {
	CreateFuncPtr funcPtr = ( *type2CreateFuncMap.find(type) ).second;
	if( funcPtr != NULL) {
		return (*funcPtr)();
	}

    throw RTSError("ModuleFactory::CreateModule(): Nie ma modulu '" + type + "'");
}

void ModuleFactory::RegisterModuleType(string type, CreateFuncPtr funcPtr) {
	type2CreateFuncMap.insert( make_pair(type, funcPtr) );
	TRACE2("ModuleFactory::RegisterModuleType", "Zarejestrowany typ ", type);
}
