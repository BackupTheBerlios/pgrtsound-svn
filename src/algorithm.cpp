#include "algorithm.h"

Algorithm::Algorithm() {
	// wyjscie audio jest zawsze
	AudioPortOut* apo = new AudioPortOut;
	apo->id = 0;
	modules.push_back(apo);
}

Algorithm::~Algorithm() {
	delete modules[0];
}

/**
 * Uruchomienie przetwarzania przez algorytm.
 * Wywolanie tej funkcji spowoduje przeliczenie bloku ramek o dlugosci
 * framesPerBlock.
 */
float* Algorithm::process() {
	int i;
	for(i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->process();
	}
	i--;
	return modulesQueue[i]->input(0).signal;
}

int Algorithm::addModule(string type) {
	Module* m = moduleFactory.createModule(type);
	
	m->id = modules.size();

	//for(int n=0; n < 10; n++) m->process();
	modules.push_back(m);

	#ifndef NDEBUG
		cout << "Modul '" << type << "' dodany do algorytmu" << endl;
	#endif
	
	return m->id;
}

void Algorithm::printInfo() {
	cout << endl << "Informacje o algorytmie: " << endl;
	for(int i = 0; i < modules.size(); i++) {
		cout << "modul id: " << modules[i]->id << "     typ modulu: " <<
			modules[i]->type << endl;
	}

	cout << "kolejnosc przetwarzania: ";
	for(int i = 0; i < modulesQueue.size(); i++) {
		cout << modulesQueue[i]->id << " ";
	}
		
	cout << endl << endl;
}

Module* Algorithm::module(int moduleId) {
	return modules[moduleId];
}

/**
 * Laczenie dwoch blokow
 */
void Algorithm::connectModules(int moduleId1, int outputId, int moduleId2, int inputId) {
	#ifndef NDEBUG
		cout << "Lacze " << 
			module(moduleId1)->type << "(" << module(moduleId1)->output(outputId).name << ") -> " <<
			module(moduleId2)->type << "(" << module(moduleId2)->input(inputId).name << ")" << endl;
	#endif
 	module(moduleId2)->input(inputId) = module(moduleId1)->output(outputId);

	/* TODO (#1#): Tu musi zachodzic sledzenie zalenznosci powiazan 
	           meidzy modulami (tworzenie grafu) */
}

/**
 * Dla celow testowych mozna za pomoca tej funckji ustawci recznie kolejnosc
 * przetwarzania modulow.
 */
void Algorithm::setQueueManually(int* order, int num) {
	modulesQueue.clear();
	for(int i = 0; i < num; i++) {
		modulesQueue.push_back( modules[ order[i] ] );
	}
}

void Algorithm::setFramesPerBlock(unsigned long fpb) {
	framesPerBlock = fpb;
	Module::framesPerBlock = fpb; // wartos widoczna we wszystkich modulach
	#ifndef NDEBUG
		cout << "Module::framesPerBlock = " << Module::framesPerBlock << endl;
	#endif

/* TODO (#1#): reinicjalizacja buforow wyjsciowych wszystkich 
               modulow */	
}

void Algorithm::setSampleRate(int sRate) {
	sampleRate = sRate;
	Module::sampleRate = sRate; // wartos widoczna we wszystkich modulach
	#ifndef NDEBUG
		cout << "Module::sampleRate = " << Module::sampleRate << endl;
	#endif
	
/* TODO (#1#): trzeba dac znac modulom ze sie zmienilo  */	
}
