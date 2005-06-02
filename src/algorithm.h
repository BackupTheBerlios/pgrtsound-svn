#ifndef ALGORITHM_H
#define ALGORITHM_H

//#include <algorithm>
#include <string>
#include <iostream>
//#include <iterator>
//#include <list>
//#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/topological_sort.hpp>

#include "modulefactory.h"


typedef std::pair<std::size_t, std::size_t> Pair;

// Boost Graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> Graph;

/**
 * Klasa bedaca najwyzsza struktura w systemie.
 * Przechowuje zastosowane moduly oraz zna sposob ich polaczenia.
 * Tym samym dopowiada skonstruowanemu algorytmowi.
 */
class Algorithm {
	public:
  		Algorithm(unsigned long framesPerBlock);
		~Algorithm();
		void	Process();
		int     AddModule(string type);
		void    ConnectModules(int moduleId1, int outputId, int moduleId2, int inputId);
		void    CreateQueue(void);
		void    SetQueueManually(int* order, int num);
		//void    SetFramesPerBlock(unsigned long fpb);
		void    SetSampleRate(int sRate);
		Module* GetModule(int moduleId) const;
		Module* GetModule(string moduleName) const;
		void    PrintInfo(void) const;
		int     GetModulesCount() const;
		void    Clear();
		void    Init();
        
	private:
		Graph				graph;
		int					sampleRate;
		unsigned long		framesPerBlock;
		ModuleFactory		moduleFactory;
		vector<Module*>		modules;
		vector<Module*>		modulesQueue;
};


/**
 * Zwraca wskaznik do modulu o zadanym identyfikatorze.
 * @param moduleId Identyfikator modulu
 */
inline Module* Algorithm::GetModule(int moduleId) const {
	return modules[moduleId];
}

/**
 * Funckja przetwarzania algorytmu.
 * Funkcja powoduje wywolanie funkcji przetwarzania wszystkich
 * wykorzystanych w budowie algorytmu modulow.
 * framesPerBlock.
 */
inline void Algorithm::Process() {
	int i;
	for(i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->Process();
	}
}


#endif // ALGORITHM_H
