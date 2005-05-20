#ifndef ALGORITHM_H
#define ALGORITHM_H

//#include <algorithm>
#include <string>
#include <iostream>
//#include <iterator>
//#include <list>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

#include "modulefactory.h"

//#include <utility>

typedef std::pair<std::size_t, std::size_t> Pair;

/**
 * Klasa bedaca najwyzsza struktura w systemie.
 * Przechowuje zastosowane moduly oraz zna sposob ich polaczenia.
 * Tym samym dopowiada skonstruowanemu algorytmowi.
 */
class Algorithm {
	public:
  		Algorithm();
		~Algorithm();
		void	Process();
		int     AddModule(string type);
		void    PrintInfo();
		void    CreateAdjacencyMatrix(void);
		void    ConnectModules(int moduleId1, int outputId, int moduleId2, int inputId);
		void    CreateQueue(void);
		void    SetQueueManually(int* order, int num);
		void    SetFramesPerBlock(unsigned long fpb);
		void    SetSampleRate(int sRate);
		Module* GetModule(int moduleId);
        
	private:
		int					sampleRate;
		unsigned long		framesPerBlock;
		ModuleFactory		moduleFactory;
		
		vector<Module*>		modules;
		vector<Module*>		modulesQueue;
        int**				adjacencyMatrix;	//  macierz s¹siedztwa [j][k] gdy 1 to po³¹czeni j -> k, gdy -1 odwrotnie
};

/**
 * Zwraca wskaznik do modulu o zadanym identyfikatorze.
 * @param Identyfikator modulu
 */
inline Module* Algorithm::GetModule(int moduleId) {
	return modules[moduleId];
}

/**
 * Uruchomienie przetwarzania przez algorytm.
 * Wywolanie tej funkcji spowoduje przeliczenie bloku ramek o dlugosci
 * framesPerBlock.
 */
inline void Algorithm::Process() {
	int i;
	for(i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->Process();
	}
}


#endif // ALGORITHM_H
