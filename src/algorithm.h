#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>

#include "modulefactory.h"
#include "tinyxml.h"
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <utility>

typedef std::pair<std::size_t,std::size_t> Pair;

/**
 * Klasa bedaca najwyzsza struktura w systemie.
 * Przechowuje zastosowane moduly oraz zna sposob ich polaczenia.
 * Tym samym dopowiada skonstruowanemu algorytmowi.
 */
class Algorithm {
	public:
  		Algorithm();
		~Algorithm();
		float*	Process();
		int     AddModule(string type);
		void    PrintInfo();
		void    CreateAdjacencyMatrix(void);
		void    ConnectModules(int moduleId1, int outputId, int moduleId2, int inputId);
		void    CreateQueue(void);
		void    SetQueueManually(int* order, int num);
		void    SetFramesPerBlock(unsigned long fpb);
		void    SetSampleRate(int sRate);
		void    LoadAlgorithmSettingsFile(const char * filename);
        void    LoadModulesFromFile(const char * filename);
        void    LoadConnectionsFromFile(const char * filename);
        void    LoadParametersFromFile(const char * filename);
		Module* GetModule(int moduleId);
        
	private:
		int					sampleRate;
		unsigned long		framesPerBlock;
		ModuleFactory		moduleFactory;
		map <string, int>	moduleName2IDMap;	// mapa asocjacyjna nazwy z id
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

#endif // ALGORITHM_H
