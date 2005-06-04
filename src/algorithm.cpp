#include "algorithm.h"

extern void SetNullBuffer(unsigned long);

Algorithm::Algorithm(unsigned long fpb) {
	framesPerBlock = fpb;
	Module::framesPerBlock = fpb;
	SetNullBuffer(fpb);

	InitAudioPorts();
}

Algorithm::~Algorithm() {
//	TRACE3("Algorithm", "Sprzatanie algorytmu (", modules.size(), " modulow)...");
//
//	delete modules[0];
//	delete modules[1];
//
//    for (int i = 2; i < modules.size(); i++) {
//        delete modules[i];
//    }
//
//	TRACE("Algorithm", "Bye!");
}

void Algorithm::InitAudioPorts() {
   	Module* module;
	ModuleId moduleId;
	
	module = new AudioPortIn;
	moduleId = add_vertex(graph);   // dodajemy do grafu
	//graph[moduleId].module = module;
	put(boost::vertex_name, graph, moduleId, module);
	moduleName2IdMap.insert( make_pair("AudioPortIn", moduleId) );
	inputPort = module;

	module = new AudioPortOut;
	moduleId = add_vertex(graph);
	//graph[moduleId].module = module;
	put(boost::vertex_name, graph, moduleId, module);
	moduleName2IdMap.insert( make_pair("AudioPortOut", moduleId) );
	outputPort = module;

	TRACE("Algorithm::Algorithm()", "Dodano moduly AudioPortIn i AudioPortOut");
}

ModuleId Algorithm::AddModule(string type, string name) {
	Module* mod = moduleFactory.CreateModule(type);
	mod->SetName(name);
	ModuleId moduleId;
	moduleId = add_vertex(graph);
	//graph[*gm] = *(moduleFactory.CreateModule(type));
	//graph[moduleId].module = mod;
	put(boost::vertex_name, graph, moduleId, mod);

	moduleName2IdMap.insert( make_pair(name, moduleId) );
	TRACE4("Algorithm", "Dodano modul '", mod->GetName(), "' typu ", mod->GetType());
	return moduleId;
}

void Algorithm::PrintInfo(void) {
	cout << "Algorithm::PrintInfo(): Informacje o algorytmie... " << endl;

	Module* mod;

	for(mod = GetFirstModule(); mod; mod = GetNextModule()) {
		cout << "    Modul: " << mod->GetName() <<
			"(" << mod->GetType() << ")" << endl;
	}

//	using namespace boost;
//	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
//	graph_traits<Graph>::edge_iterator ei, ei_end;
//	typedef property_map<Graph, vertex_index_t>::type IndexMap;
//
//	cout << "Algorithm::PrintInfo(): Informacje o algorytmie... " << endl;
//
//	for(int i = 0; i < modules.size(); i++) {
//		cout << "    Modul id: " << modules[i]->GetID() <<
//		    "  " << modules[i]->GetName() <<
//			"(" << modules[i]->GetType() << ")" << endl;
//	}
//
//    cout << "    vertices = ";
//    std::pair<vertex_iter, vertex_iter> vp;
//	IndexMap index = get(vertex_index, graph);
//	for (vp = vertices(graph); vp.first != vp.second; ++vp.first)
//      cout << index[*vp.first] <<  " ";
//    cout << endl;
//
//    cout << "    edges = ";
//    for (tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei)
//        std::cout << "(" << index[source(*ei, graph)]
//                  << "," << index[target(*ei, graph)] << ") ";
//    std::cout << std::endl;
//
//	cout << "    kolejnosc przetwarzania: ";
//	for(int i = 0; i < modulesQueue.size(); i++) {
//		cout << modulesQueue[i]->GetID() << " ";
//	}
//	cout << endl;
}


/**
 * Laczenie dwoch modulow
 */
ConnectionId Algorithm::ConnectModules(ModuleId moduleId1, int outputId, ModuleId moduleId2, int inputId) {
	#ifndef NDEBUG
		cout << "    Lacze '" <<
			GetModule(moduleId1)->GetName() << "'." << GetModule(moduleId1)->GetOutput(outputId)->GetName() << " -> '" <<
			GetModule(moduleId2)->GetName() << "'." << GetModule(moduleId2)->GetInput(inputId)->GetName() << endl;
	#endif

	GetModule(moduleId2)->ConnectInputTo(
		inputId,
		GetModule(moduleId1)->GetOutput(outputId)->GetSignal()
	);

	ConnectionDescription c;
	ConnectionId cId;
	c = add_edge(moduleId1, moduleId2, graph);
	cId = c.first;
	graph[cId].sourceId = outputId;
	graph[cId].destinationId = inputId;
	graph[cId].source = GetModule(moduleId1)->GetOutput(outputId);
	graph[cId].sink = GetModule(moduleId2)->GetInput(inputId);
	
	return cId;
}

ConnectionId Algorithm::ConnectModules(string moduleName1, int outputId, string moduleName2, int inputId) {
	ModuleId moduleId1 = ( *moduleName2IdMap.find(moduleName1) ).second;
	ModuleId moduleId2 = ( *moduleName2IdMap.find(moduleName2) ).second;

	return ConnectModules(moduleId1, outputId, moduleId2, inputId);
}

void Algorithm::SetSampleRate(float sRate) {
	sampleRate = sRate;
	Module::sampleRate = sRate; // wartos widoczna we wszystkich modulach

	TRACE2("Algorithm", "Module::sampleRate = ", Module::sampleRate);
}

void Algorithm::CreateQueue(void) {
	TRACE("Algorithm", "Tworzenie kolejki modulow...");

	using namespace boost;

	typedef vector< ModuleId > container;
	container c;
	property_map<Graph, vertex_index_t>::type index = get(vertex_index, graph);
	property_map<Graph, vertex_name_t>::type name = get(vertex_name, graph);

	// initialize the vertex_index property values
	graph_traits<Graph>::vertex_iterator vi, vend;
	graph_traits<Graph>::vertices_size_type cnt = 0;
	for(tie(vi,vend) = vertices(graph); vi != vend; ++vi)
	    put(index, *vi, cnt++);

	modulesQueue.clear();
	topological_sort(graph, std::back_inserter(c));

	std::cout << "A topological ordering: ";
	for (container::reverse_iterator ii = c.rbegin(); ii != c.rend(); ++ii) {
        cout << "'" << name[*ii]->GetName() << "' ";
        modulesQueue.push_back(name[*ii]);
	}
    cout << endl;
}

int Algorithm::GetModulesCount() const {
	return num_vertices(graph);
}

void  Algorithm::Clear() {
	TRACE("Algorithm::Clear()", "Czyszcze algorytm...");

//	for(unsigned int i = 0; i < modules.size(); i++) {
//		cout << "Usuwam vertex " << i << endl;
//	}
//
//	for (unsigned int i = 2; i < modules.size(); i++) {
//		cout << "Usuwam modul " << i << endl;
//        delete GetModule(i);
//    }

	modulesQueue.clear();
	graph.clear();
	moduleName2IdMap.clear();
	InitAudioPorts();
	
	TRACE("Algorithm::Clear()", "Algorytm wyczyszczony");
}

void Algorithm::Init() {
    TRACE("Algorithm::Init()", "Inicjalizacja modulow...");
    
   	typedef boost::property_map<Graph, boost::vertex_name_t>::type Modules;
    Modules index = get(boost::vertex_name, graph);

    pair<ModuleIterator, ModuleIterator> vp;

	for (vp = vertices(graph); vp.first != vp.second; ++vp.first) {
		index[*vp.first]->Init();
	}

    TRACE("Algorithm::Init()", "Inicjalizacja modulow zakonczona");
}

Module* Algorithm::GetModule(string moduleName) const {
	ModuleId moduleId = ( *moduleName2IdMap.find(moduleName) ).second;
	return GetModule(moduleId);
}

/**
 * Zwraca wskaznik do modulu o zadanym identyfikatorze.
 * @param moduleId Identyfikator modulu
 */
Module* Algorithm::GetModule(ModuleId moduleId) const {
	return boost::get(boost::vertex_name, graph, moduleId);
}

Module* Algorithm::GetInputPort() const {
	return inputPort;
}

Module* Algorithm::GetOutputPort() const {
	return outputPort;
}

Module* Algorithm::GetFirstModule() {
    boost::tie(moduleIterator, moduleIteratorLast) = vertices(graph);
    return boost::get(boost::vertex_name, graph, *moduleIterator);
}

Module* Algorithm::GetNextModule() {
	if(++moduleIterator != moduleIteratorLast)
	    return boost::get(boost::vertex_name, graph, *moduleIterator);
	else
		return NULL;
}

void Algorithm::DeleteModule(ModuleId moduleId) {
	delete GetModule(moduleId);
	boost::clear_vertex(moduleId, graph);
}

void Algorithm::DeleteConnection(ConnectionId connectionId) {
	extern float* nullBuffer;

	graph[connectionId].sink->SetSignal(nullBuffer); // rozlaczamy wejscie modulu 2 ???
	boost::remove_edge(connectionId, graph);
}

