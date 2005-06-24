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
 Tworzy modul wybranego typu.
 Po pomyslnym utworzeiu modulu zostaje zwrocony wskaznik od niego. Usuwaniem
 utoworznych modulow musi zajac sie programista.
 @param type Typ modulu.
*/
Module* ModuleFactory::CreateModule(string type) {
	CreateFuncPtr funcPtr = NULL;
	funcPtr = ( *type2CreateFuncMap.find(type) ).second;
	if( funcPtr != NULL) {
		// tworzymu modul danego typu
		return (*funcPtr)();
	}

    throw RTSError("ModuleFactory::CreateModule(): Nie ma modulu '" + type + "'");
}

/**
 Zarejstroawnie typu modulu.
 Aby modul stal sie dostepny w systemie, konieczne jest zaresjtrowanie jego
 typu oraz zdefiniowanie funkcji ktora zajmuje sie tworzeniem obiektow tego typu.
 @param type Typ modulu do zarejstrowania - musi byc unikatowy w calym systemie
 @param funcPtr wskazink do funkcji tworzacej moduly rejestrowanego typu
*/
void ModuleFactory::RegisterModuleType(string type, CreateFuncPtr funcPtr) {
	type2CreateFuncMap.insert( make_pair(type, funcPtr) );
	TRACE3("ModuleFactory::RegisterModuleType()", "Zarejestrowany typ '", type, "'");
}
