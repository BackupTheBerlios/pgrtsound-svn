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


class RTSError : public runtime_error {
    public:
        RTSError (const string& msg ="") : runtime_error(msg) {}
    };

/**
 * Klasa bedaca najwyzsza struktura w systemie.
 * Przechowuje zastosowane moduly oraz zna sposob ich polaczenia.
 * Tym samym dopowiada skonstruowanemu algorytmowi.
 */
class Algorithm {
	public:
		vector<Module*>   modules;
		vector<Module*>   modulesQueue;
        int     **adjacencyMatrix;	//  macierz s¹siedztwa [j][k] gdy 1 to po³¹czeni j -> k, gdy -1 odwrotnie


  		Algorithm();
		~Algorithm();
		float*	Process();
		int     AddModule(string type);
		void    PrintInfo();
		//Module* module(int moduleId);
		void    CreateAdjacencyMatrix(void);
		void    ConnectModules(int moduleId1, int outputId, int moduleId2, int inputId);
		void    CreateQueue(void);
		void    SetQueueManually(int* order, int num);
		void    SetFramesPerBlock(unsigned long fpb);
		void    SetSampleRate(int sRate);
        void    LoadModulesFromFile(const char * filename);
        void    LoadConnectionsFromFile(const char * filename);
        void    LoadParametersFromFile(const char * filename);
        
	private:
   		//unsigned long currentFrame;
		//unsigned long currentBlock;
		int               sampleRate;
		unsigned long     framesPerBlock;
		ModuleFactory     moduleFactory;
		map <string, int> moduleMap;     // mapa asocjacyjna nazwy z id
};

#endif // ALGORITHM_H
