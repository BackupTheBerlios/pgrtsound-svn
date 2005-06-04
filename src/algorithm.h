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
#include <boost/property_map.hpp>

#include "modulefactory.h"

typedef std::pair<std::size_t, std::size_t> Pair;

struct Connection {
   	Output* source;
	Input* sink;
	int sourceId;
	int destinationId;
};

typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS,
	boost::property<boost::vertex_index_t, int,
	boost::property<boost::vertex_name_t, Module*> >, Connection> Graph;
typedef Graph::vertex_descriptor ModuleId;
typedef Graph::edge_descriptor ConnectionId;
typedef std::pair<ConnectionId, bool> ConnectionDescription;
typedef boost::graph_traits<Graph>::vertex_iterator ModuleIterator;
typedef boost::graph_traits<Graph>::edge_iterator ConnectionIterator;

/**
 * Klasa bedaca najwyzsza struktura w systemie.
 * Przechowuje zastosowane moduly oraz zna sposob ich polaczenia.
 * Tym samym dopowiada skonstruowanemu algorytmowi.
 */
class Algorithm {
	public:
  		Algorithm(unsigned long framesPerBlock);
		~Algorithm();
		void Process();
		ModuleId AddModule(string type, string name);
		ConnectionId ConnectModules(ModuleId moduleId1, int outputId, ModuleId moduleId2, int inputId);
		ConnectionId ConnectModules(string moduleName1, int outputId, string moduleName2, int inputId);
		void    Clear();
		void    Init();
		void    PrintInfo();
		void    CreateQueue();
		void 	SetSampleRate(float sRate);
		Module* GetModule(ModuleId moduleId) const;
		Module* GetModule(string moduleName) const;
		int     GetModulesCount() const;
		Module* GetInputPort() const;
		Module* GetOutputPort() const;
		Module* GetFirstModule();
		Module* GetNextModule();
		void    DeleteModule(ModuleId moduleId);
		void    DeleteConnection(ConnectionId connectionI);

	private:
		Graph					graph;
		float					sampleRate;
		unsigned long			framesPerBlock;
		ModuleFactory			moduleFactory;
		//vector<Module*>			modules;
		vector<Module*>			modulesQueue;
		map<string, ModuleId>	moduleName2IdMap; // mapa asocjacyjna nazwy z id
		Module*					inputPort;
		Module*					outputPort;
		ModuleIterator		moduleIterator;
		ModuleIterator		moduleIteratorLast;
		
		void InitAudioPorts();
};

/**
 * Funckja przetwarzania algorytmu.
 * Funkcja powoduje wywolanie funkcji przetwarzania wszystkich
 * wykorzystanych w budowie algorytmu modulow.
 * framesPerBlock.
 */
inline void Algorithm::Process() {
	for(unsigned int i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->Process();
	}
}

#endif // ALGORITHM_H
