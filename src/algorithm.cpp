#include "algorithm.h"

extern void SetNullBuffer(unsigned long);

Algorithm::Algorithm(unsigned long fpb) {
	Module* module;
	
	framesPerBlock = fpb;
	Module::framesPerBlock = fpb;
	SetNullBuffer(fpb);

	// wyjscie i wyjscie audio jest zawsze
	module = new AudioPortIn;
	module->SetID(0);
	modules.push_back(module);
	add_vertex(graph);   // dodajemy do grafu (wierzcholek 0)

	module = new AudioPortOut;
	module->SetID(1);
	modules.push_back(module);
	add_vertex(graph);// wierzcholek 1

	TRACE("Algorithm::Algorithm()", "Dodano moduly AudioPortIn i AudioPortOut");
}

Algorithm::~Algorithm() {
	TRACE3("Algorithm", "Sprzatanie algorytmu (", modules.size(), " modulow)..."); 

	delete modules[0];
	delete modules[1];

    for (int i = 2; i < modules.size(); i++) {
        delete modules[i];
    } 

	TRACE("Algorithm", "Bye!");
}

int Algorithm::AddModule(string type) {
	Module* mod = moduleFactory.CreateModule(type);
	
	for (int m = 0;m < modules.size(); m++)
	   if (modules[m]->GetID() == modules.size())
	       TRACE("Algorithm::AddModule", "B³êdne ID");
	
	mod->SetID( modules.size() );
	modules.push_back(mod);
	// dodajemy do grafu
	add_vertex(graph);

	TRACE5("Algorithm", "Dodano modul typu ", mod->GetType(), "(", mod->GetID(), ")");
	
	return mod->GetID();
}

void Algorithm::PrintInfo(void) const {
	using namespace boost;
	typedef graph_traits<Graph>::vertex_iterator vertex_iter;
	graph_traits<Graph>::edge_iterator ei, ei_end;
	typedef property_map<Graph, vertex_index_t>::type IndexMap;
	
	cout << "Algorithm::PrintInfo(): Informacje o algorytmie... " << endl;

	for(int i = 0; i < modules.size(); i++) {
		cout << "    Modul id: " << modules[i]->GetID() <<
		    "  " << modules[i]->GetName() <<
			"(" << modules[i]->GetType() << ")" << endl;
	}
	
    cout << "    vertices = ";
    std::pair<vertex_iter, vertex_iter> vp;
	IndexMap index = get(vertex_index, graph);
	for (vp = vertices(graph); vp.first != vp.second; ++vp.first)
      cout << index[*vp.first] <<  " ";
    cout << endl;

    cout << "    edges = ";
    for (tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei)
        std::cout << "(" << index[source(*ei, graph)]
                  << "," << index[target(*ei, graph)] << ") ";
    std::cout << std::endl;

	cout << "    kolejnosc przetwarzania: ";
	for(int i = 0; i < modulesQueue.size(); i++) {
		cout << modulesQueue[i]->GetID() << " ";
	}
	cout << endl;
}


/**
 * Laczenie dwoch blokow
 */
void Algorithm::ConnectModules(int moduleId1, int outputId, int moduleId2, int inputId) {
    if (moduleId1 >= modules.size())
		throw RTSError("Algorithm::ConnectModules(): Bledne polaczenie. Modul (od) o takim id nie istnieje.");
    if (moduleId2 >= modules.size())
		throw RTSError("Algorithm::ConnectModules(): Bledne polaczenie. Modul (do) o takim id nie istnieje.");
		
	//#ifndef NDEBUG
	//	cout << "    Lacze " <<
	//		modules[moduleId1]->GetName() << "(" << modules[moduleId1]->GetOutput(outputId)->GetName() << ") -> " <<
	//		modules[moduleId2]->GetName() << "(" << modules[moduleId2]->GetInput(inputId)->GetName() << ")" << endl;
	//#endif

	modules[moduleId2]->ConnectInputTo(inputId,
			modules[moduleId1]->GetOutput(outputId)->GetSignal(),
            moduleId1,
            outputId);
  	
	add_edge(moduleId1, moduleId2, graph);
}

/**
 * Dla celow testowych mozna za pomoca tej funckji ustawci recznie kolejnosc
 * przetwarzania modulow.
 */
void Algorithm::SetQueueManually(int* order, int num) {
	TRACE("Algorithm", "Reczne uxtawianie kolejki... ");

	modulesQueue.clear();

	for(int i = 0; i < num; i++) {
		modulesQueue.push_back( modules[ order[i] ] );
	}

	TRACE("Algorithm", "Kolejka ustawiona");
}

//void Algorithm::SetFramesPerBlock(unsigned long fpb) {
//	framesPerBlock = fpb;
//
//	Module::framesPerBlock = fpb; // wartos widoczna we wszystkich modulach
//	TRACE2("Algorithm", "Module::framesPerBlock = ", Module::framesPerBlock);
//
//	SetNullBuffer(Module::framesPerBlock);
//
///* TODO (#1#): reinicjalizacja buforow wyjsciowych wszystkich
//               modulow */
//}

void Algorithm::SetSampleRate(int sRate) {
	sampleRate = sRate;
	Module::sampleRate = sRate; // wartos widoczna we wszystkich modulach

	TRACE2("Algorithm", "Module::sampleRate = ", Module::sampleRate);

/* TODO (#1#): trzeba dac znac modulom ze sie zmienilo  */
}

void Algorithm::CreateQueue(void) {
	TRACE("Algorithm", "Tworzenie kolejki modulow...");
	
	using namespace boost;

	typedef graph_traits<Graph>::vertex_descriptor Vertex;
	typedef std::vector< Vertex > container;
	typedef property_map<Graph, vertex_index_t>::type IndexMap;

	container c;
	IndexMap index = get(vertex_index, graph);

	modulesQueue.clear();
	
    topological_sort(graph, std::back_inserter(c));

    cout << "A topological ordering: ";
    for (container::reverse_iterator ii = c.rbegin(); ii != c.rend(); ++ii) {
        cout << index[*ii] << " ";
		modulesQueue.push_back( modules[index[*ii]]  );
    }
    cout << endl;
}

int Algorithm::GetModulesCount() const {
	return modules.size();
}

void  Algorithm::Clear() {
	TRACE("Algorithm::Clear()", "Czyszcze algorytm...");

	for(unsigned int i = 0; i < modules.size(); i++) {
		cout << "Usuwam vertex " << i << endl;
	}
	
	for (unsigned int i = 2; i < modules.size(); i++) {
		cout << "Usuwam modul " << i << endl;
        delete GetModule(i);
    }
	
	modulesQueue.clear();
	modules.clear();
	graph.clear();
	
	Module* module;

	// wyjscie i wyjscie audio jest zawsze
	module = new AudioPortIn;
	module->SetID(0);
	modules.push_back(module);
	add_vertex(graph);   // dodajemy do grafu (wierzcholek 0)

	module = new AudioPortOut;
	module->SetID(1);
	modules.push_back(module);
	add_vertex(graph);// wierzcholek 1
	
	TRACE("Algorithm::Clear()", "Algorytm wyczyszczony");
}

void Algorithm::Init() {
    TRACE("Algorithm::Init()", "Inicjalizacja modulow...");
	for(unsigned int i = 0; i < modules.size(); i++) {
		modules[i]->Init();
	}
    TRACE("Algorithm::Init()", "Inicjalizacja modulow zakonczona");
}


Module* Algorithm::GetModule(string moduleName) const {
    for(unsigned int i = 0; i < modules.size(); i++) {
		if (moduleName == modules[i]->GetName()) return modules[i];
	}    
    return NULL;
}
