/*! \mainpage System przetwarznia dzwieku w czasie rzeczywsitym
 *
 * \section intro_sec Wprowadzenie
 *
 * Zadaniem systemu jest przetwarzanie dzwikeu w czasie rzeczywistym.
 * To jest test.
 *
 * \section install_sec Instalacja
 *
 * \subsection step1 Krok 1: Rozpakowanie archiwow
 *
 */

#ifndef ALGORITHM_H
#define ALGORITHM_H

//#include <algorithm>
#include <string>
#include <iostream>
//#include <iterator>
//#include <list>
//#include <utility>

//#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/graph_traits.hpp>
#include <boost/graph/topological_sort.hpp>
//#include <boost/property_map.hpp>

#include "modulefactory.h"

typedef std::pair<std::size_t, std::size_t> Pair;

// struktua zwiazana z krawedzia grafu
struct Connection {
   	Module* sourceModule;
	Module* destinationModule;
	int sourceOutputId;
	int destinationInputId;
};

// struktura zwiazana z wierzcholkiem grafu
struct GraphModule {
	Module* module;
};

// graf
typedef boost::adjacency_list<boost::listS, boost::listS, boost::bidirectionalS,
	boost::property<boost::vertex_index_t, std::size_t, GraphModule >, Connection> Graph;
typedef Graph::vertex_descriptor ModuleId;
typedef Graph::edge_descriptor ConnectionId;
typedef std::pair<ConnectionId, bool> ConnectionDescription;
typedef boost::graph_traits<Graph>::vertex_iterator ModuleIdIterator;
typedef boost::graph_traits<Graph>::edge_iterator ConnectionIdIterator;

/**
 * Klasa bedaca najwyzsza struktura w systemie.
 * Przechowuje zastosowane moduly oraz zna sposob ich polaczenia.
 * Tym samym dopowiada skonstruowanemu algorytmowi.
 */
class Algorithm {
	public:
  		Algorithm(unsigned long framesPerBlock);
		~Algorithm();
		void			Process();
		void			Clear();
		void			Init();
		void			PrintInfo();
		void			CreateQueue();
		void			SetSampleRate(float sRate);
		void            SetName(string newName);
		string GetName();
		void SetBlockSize(unsigned long newBlockSize);
		Module* GetModule(ModuleId moduleId) const;
		Module* GetModule(string moduleName) const;
		ModuleId GetModuleId(string moduleName) const;
		Connection* GetConnection(ConnectionId connectionId);
		int GetModuleCount() const;
		int GetConnectionCount();
		Module* GetInputPort() const;
		Module* GetOutputPort() const;
		ModuleIdIterator ModuleIdIteratorBegin();
		ModuleIdIterator ModuleIdIteratorEnd();
		ConnectionIdIterator ConnectionIdIteratorBegin();
		ConnectionIdIterator ConnectionIdIteratorEnd();
		void          DeleteModule(ModuleId moduleId);
		void          DeleteConnection(ConnectionId connectionId);
		ModuleId      AddModule(string type, string name);
		ConnectionId  ConnectModules(ModuleId moduleId1, int outputId,
							ModuleId moduleId2, int inputId);
		ConnectionId  ConnectModules(string moduleName1, int outputId,
							string moduleName2, int inputId);
   		// No description
		void PrintEdges();

	private:
		void InitAudioPorts();
		
		string                  name;
		Graph					graph;
		float					sampleRate;
		unsigned long			framesPerBlock;
		ModuleFactory			moduleFactory;
		std::vector<Module*>	modulesQueue;
		std::map<string, ModuleId>	moduleName2IdMap; // mapa asocjacyjna nazwy z id
		Module*					inputPort;
		Module*					outputPort;
		ModuleIdIterator		moduleIterator, moduleIteratorLast;
		ConnectionIdIterator	connectionIterator, connectionIteratorLast;
};

/**
 * Funckja przetwarzania algorytmu.
 * Funkcja powoduje wywolanie funkcji przetwarzania wszystkich
 * wykorzystanych w algorytmie modulow.
 */
inline void Algorithm::Process() {
	for(unsigned int i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->Process();
	}
}

#endif // ALGORITHM_H
