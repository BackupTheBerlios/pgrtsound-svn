#ifndef ALGORITHM_H
#define ALGORITHM_H


#include <string>

#include "modulefactory.h"
#include "tinyxml.h"
#include <boost/config.hpp>
#include <iostream>
#include <list>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <iterator>
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
		
        int     **adjacencyMatrix; 		    //  macierz s¹siedztwa [j][k] gdy 1 to po³¹czeni j -> k, gdy -1 odwrotnie      
		float   *process();
		int     addModule(string type);
		void    printInfo();
		Module* module(int moduleId);
		void    CreateAdjacencyMatrix(void);
		void    connectModules(int moduleId1, int outputId, int moduleId2, int inputId);
		void    CreateQueue(void);
		void    setQueueManually(int* order, int num);
		void    setFramesPerBlock(unsigned long fpb);
		void    setSampleRate(int sRate); 		
        void    LoadModulesFromFile(const char * filename);
        void    LoadConnectionsFromFile(const char * filename);
        void    LoadParametersFromFile(const char * filename);
	private:
         

        
		//unsigned long currentFrame;
		//unsigned long currentBlock;
		int               sampleRate;
		unsigned long     framesPerBlock;
		ModuleFactory     moduleFactory;	
		vector<Module*>   modules;
		vector<Module*>   modulesQueue;
};

#endif // ALGORITHM_H
