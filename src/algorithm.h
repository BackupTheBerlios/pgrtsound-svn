#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "modulefactory.h"

/**
 * Klasa bedaca najwyzsza struktura w systemie.
 * Przechowuje zastosowane moduly oraz zna sposob ich polaczenia.
 * Tym samym dopowiada skonstruowanemu algorytmowi.
 */
class Algorithm {
	public:
		Algorithm();
		~Algorithm();
		
		float* process();
		int addModule(string type);
		void printInfo();
		Module* module(int moduleId);
		void connectModules(int moduleId1, int outputId, int moduleId2, int inputId);
		void setQueueManually(int* order, int num);
		void setFramesPerBlock(unsigned long fpb);
		void setSampleRate(int sRate); 
		
	private:
		//unsigned long currentFrame;
		//unsigned long currentBlock;
		int sampleRate;
		unsigned long framesPerBlock;
		ModuleFactory moduleFactory;	
		vector<Module*> modules;
		vector<Module*> modulesQueue;
};

#endif // ALGORITHM_H
