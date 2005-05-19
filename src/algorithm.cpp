#include "algorithm.h"

Algorithm::Algorithm() {
	// wyjscie i wyjscie audio jest zawsze
	AudioPortIn* api = new AudioPortIn;
	api->SetID(0);
	modules.push_back(api);

	AudioPortOut* apo = new AudioPortOut;
	apo->SetID(1);
	modules.push_back(apo);
	
	TRACE("Algorithm::Algorithm()", "Dodano AudioPortIn i AudioPortOut");
}

Algorithm::~Algorithm() {
	TRACE3("Algorithm", "Sprzatanie algorytmu (", modules.size(), " modulow)..."); 

	//delete modules[0];
	//delete modules[1];

    for (int i = 0; i < modules.size(); i++) {
        delete adjacencyMatrix[i];
        delete modules[i];
    } 
    delete adjacencyMatrix;
	
	TRACE("Algorithm", "Bye!");
}

/**
 * Uruchomienie przetwarzania przez algorytm.
 * Wywolanie tej funkcji spowoduje przeliczenie bloku ramek o dlugosci
 * framesPerBlock.
 */
void Algorithm::Process() {
	int i;
	for(i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->Process();
	}
}

int Algorithm::AddModule(string type) {
	Module* m = moduleFactory.CreateModule(type);
	
	m->SetID( modules.size() );
	modules.push_back(m);

	TRACE5("Algorithm", "Modul typu ", type, " o id = ", m->GetID(), " dodany do algorytmu");
	
	return m->GetID();
}

void Algorithm::PrintInfo() {
	cout << endl << "Informacje o algorytmie: " << endl;
	for(int i = 0; i < modules.size(); i++) {
		cout << "modul id: " << modules[i]->GetID() << "     typ modulu: " <<
			modules[i]->GetType() << endl;
	}

    cout << endl;

	cout << "Macierz przetwarzaina" << endl;
	for(int i = 0; i < modules.size(); i++) {
        for(int j = 0; j < modules.size(); j++) {
		  cout << adjacencyMatrix[i][j] << "  ";
        }
        cout << endl;
	}

    cout << endl;

	cout << "kolejnosc przetwarzania: ";
	for(int i = 0; i < modulesQueue.size(); i++) {
		cout << modulesQueue[i]->GetID() << " ";
	}		
	cout << endl << endl;
}


/**
 * Laczenie dwoch blokow
 */
void Algorithm::ConnectModules(int moduleId1, int outputId, int moduleId2, int inputId) {
    if (moduleId1 >= modules.size())
		throw RTSError("Algorithm::ConnectModules(): Bledne polaczenie. Modul o takim id nie istnieje.");
    if (moduleId2 >= modules.size())
		throw RTSError("Algorithm::ConnectModules(): Bledne polaczenie. Modul o takim id nie istnieje.");

	#ifndef NDEBUG
		cout << "Lacze " <<
			modules[moduleId1]->GetName() << "(" << modules[moduleId1]->GetOutput(outputId)->GetName() << ") -> " <<
			modules[moduleId2]->GetName() << "(" << modules[moduleId2]->GetInput(inputId)->GetName() << ")" << endl;
		cout << "Lacze " <<
			moduleId1 << "(" << modules[moduleId1]->GetOutput(outputId)->GetID() << ") -> " <<
			moduleId2 << "(" << modules[moduleId2]->GetInput(inputId)->GetID() << ")" << endl;
	#endif

	modules[moduleId2]->ConnectInputTo( inputId, modules[moduleId1]->GetOutput(outputId)->GetSignal() );
    adjacencyMatrix[moduleId1][moduleId2] = 1;
    adjacencyMatrix[moduleId2][moduleId1] = -1;
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

void Algorithm::SetFramesPerBlock(unsigned long fpb) {
	framesPerBlock = fpb;

	Module::framesPerBlock = fpb; // wartos widoczna we wszystkich modulach
	TRACE2("Algorithm", "Module::framesPerBlock = ", Module::framesPerBlock);

/* TODO (#1#): reinicjalizacja buforow wyjsciowych wszystkich
               modulow */
}

void Algorithm::SetSampleRate(int sRate) {
	sampleRate = sRate;
	Module::sampleRate = sRate; // wartos widoczna we wszystkich modulach

	TRACE2("Algorithm", "Module::sampleRate = ", Module::sampleRate);

/* TODO (#1#): trzeba dac znac modulom ze sie zmienilo  */
}

/**
 * Funckja tworz¹ca macierz s¹siedztwa
*/
void Algorithm::CreateAdjacencyMatrix(void) {
    TRACE("Algorithm", "Tworzenie macierzy sasiedztwa...");

    adjacencyMatrix = new int*[modules.size()];
    for (int i = 0; i < modules.size(); i++)
    {
        adjacencyMatrix[i]  = new int[modules.size()];
    } 
    
    for(int i = 0; i < modules.size(); i++) {
        for(int j = 0; j < modules.size(); j++) {
		  adjacencyMatrix[i][j] = 0;
        }
	}
	
	TRACE("Algorithm", "Macierzy sasiedztwa utworzona");
}

void Algorithm::CreateQueue(void) {
	TRACE("Algorithm", "Tworzenie kolejki modulow...");
    
    int edgeCount = 0;
    int edgeIndex = 0;
    for(int i = 0; i < modules.size(); i++) {
        for(int j = i; j < modules.size(); j++) {
		  if (adjacencyMatrix[i][j] != 0) edgeCount++;         
        }
	} 
	
	TRACE2("Algorithm", "Liczba krawedzi: ", edgeCount);
   	
    using namespace boost;

    typedef adjacency_list<vecS, vecS, directedS,property<vertex_color_t, default_color_type> > Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    
    Pair edges[edgeCount];
    
    //edgeCount = 0;
    for(int i = 0; i < modules.size(); i++) {
        for(int j = i; j < modules.size(); j++) {
		  if (adjacencyMatrix[i][j] ==1)
		  {          
              //cout << "Pair(" << i << "," << j << ")" << endl;
		      edges[edgeIndex] = Pair (i,j);
		      edgeIndex++;
          }
          if (adjacencyMatrix[i][j] ==-1)
		  {
              //cout << "Pair(" << j << "," << i << ")" << endl;
		      edges[edgeIndex] = Pair (j,i);
		      edgeIndex++;
          }          
        }        
	} 
	
	TRACE("Algorithm", "Robie graf...");
	
    //Graph G(5);
    Graph G(edgeCount);
    for (std::size_t j = 0; j < edgeCount; ++j) {
		//cout << "add_edge: " << edges[j].first << " " << edges[j].second << endl;
        add_edge(edges[j].first, edges[j].second, G);
	}
    
    boost::property_map<Graph, vertex_index_t>::type id = get(vertex_index, G);

    typedef std::vector< Vertex > container;
    container c;
    topological_sort(G, std::back_inserter(c));

    modulesQueue.clear();
    
    std::cout << "A topological ordering: ";
    for (container::reverse_iterator ii = c.rbegin(); ii != c.rend(); ++ii)
    {
        std::cout << id[*ii] << " "; 
		modulesQueue.push_back( modules[id[*ii]]  );
    }
    std::cout << std::endl;  
    
    TRACE("Algorithm", "Kolejka utworzona");
}
