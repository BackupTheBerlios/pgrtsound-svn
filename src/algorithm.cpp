#include "algorithm.h"

Algorithm::Algorithm() {
	// wyjscie audio jest zawsze
	AudioPortOut* apo = new AudioPortOut;
	apo->id = 0;
	modules.push_back(apo);	
}

Algorithm::~Algorithm() {
	TRACE3("Algorithm", "Sprzatanie algorytmu (", modules.size(), " modulow)..."); 

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
float* Algorithm::Process() {
	int i;
	for(i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->Process();
	}
	i--;
	return modulesQueue[i]->inputs[0]->signal;
}

int Algorithm::AddModule(string type) {
	Module* m = moduleFactory.CreateModule(type);
	
	m->id = modules.size();
	modules.push_back(m);

	TRACE3("Algorithm", "Modul '", type, "' dodany do algorytmu");
	
	return m->id;
}

void Algorithm::PrintInfo() {
	cout << endl << "Informacje o algorytmie: " << endl;
	for(int i = 0; i < modules.size(); i++) {
		cout << "modul id: " << modules[i]->id << "     typ modulu: " <<
			modules[i]->type << endl;
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
		cout << modulesQueue[i]->id << " ";
	}		
	cout << endl << endl;
}
//
//Module* Algorithm::module(int moduleId) {
//	return modules[moduleId];
//}

/**
 * Laczenie dwoch blokow
 */
void Algorithm::ConnectModules(int moduleId1, int outputId, int moduleId2, int inputId) {
    if (moduleId1>=modules.size()) throw RTSError("Bledne polaczenie. Modul nie istnieje.");
    if (moduleId2>=modules.size()) throw RTSError("Bledne polaczenie. Modul nie istnieje.");
    
	#ifndef NDEBUG
		cout << "Lacze " << 
			modules[moduleId1]->type << "(" << modules[moduleId1]->outputs[outputId]->name << ") -> " <<
			modules[moduleId2]->type << "(" << modules[moduleId2]->inputs[inputId]->name << ")" << endl;
		cout << "Lacze " << 
			moduleId1 << "(" << modules[moduleId1]->outputs[outputId]->id << ") -> " <<
			moduleId2 << "(" << modules[moduleId2]->inputs[inputId]->id << ")" << endl;
	#endif
	
	modules[moduleId2]->ConnectInputTo( inputId, modules[moduleId1]->outputs[outputId]->signal );
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
 * Funckja wczytuj¹ca do pamiêci modu³y 
 * @param Nazwa plik zaierajacego Algorytm
*/
void Algorithm::LoadModulesFromFile(const char * filename) { 
    TRACE("Algorithm", "Wczytywanie modulow...");      

    TiXmlNode* node                     = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* moduleElements        = 0;
    TiXmlElement* moduleXML             = 0;
    
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
        //cout << "Could not load file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
        //exit( 1 );
        throw RTSError("Nie mo¿na wczytaæ pliku " + (string)filename + " Error: " + doc.ErrorDesc());
        
    } 

    node = doc.FirstChild( "algorithm" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );
   
    node = projectElement->FirstChildElement("modules");
    assert( node );
    moduleElements = node->ToElement();
    assert( moduleElements  );

    // stworzenie wszystkich modu³ów
    //pierw g³oœnika
    moduleMap.insert(make_pair("0",0));
    //a potem reszte :)
    for( moduleXML = moduleElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {    
       moduleMap.insert(make_pair(moduleXML->Attribute("name"),AddModule(moduleXML->Attribute("type"))));
	}
	
	TRACE("Algorithm", "Moduly wczytane");
}

/**
 * Funckja wczytuj¹ca do pamiêci parametry modu³ów
 * @param Nazwa plik zaierajacego Algorytm
*/
void Algorithm::LoadParametersFromFile(const char * filename) { 
    TRACE("Algorithm", "Wczytywanie parametrow...");                         
   
    TiXmlNode*    node                  = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* moduleElements        = 0;
    TiXmlElement* moduleXML             = 0;        
    
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
        //cout << "Could not load file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
        //exit( 1 );
        throw RTSError("Nie mo¿na wczytaæ pliku " + (string)filename + " Error: " + doc.ErrorDesc());
    } 

    node = doc.FirstChild( "algorithm" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );
      
    node = projectElement->FirstChildElement("parameters");
    assert( node );
    moduleElements = node->ToElement();
    assert( moduleElements  );

    for( moduleXML = moduleElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {
        modules[ (*moduleMap.find(moduleXML->Attribute("name"))).second ]->SetParam(atoi(moduleXML -> Attribute("number")), atof(moduleXML -> Attribute("value")));
    }
    
 	TRACE("Algorithm", "Parametry wczytane");
}

/**
 * Funckja wczytuj¹ca do pamiêci powi¹zania miêdzy modu³ami 
 * @param Nazwa plik zaierajacego Algorytm
*/
void Algorithm::LoadConnectionsFromFile(const char * filename) {       
    //Zmienne do parsowania XML
    TRACE("Algorithm", "Wczytywanie polaczen...");
     
    string strTemp;
    
    TiXmlNode*    node                  = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* connectionElements    = 0;
    TiXmlElement* moduleXML             = 0;
    
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
        //cout << "Could not load file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
        //exit( 1 );
        throw RTSError("Nie mo¿na wczytaæ pliku " + (string)filename + " Error: " + doc.ErrorDesc());
    } 

    node = doc.FirstChild( "algorithm" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );

	node = projectElement->FirstChildElement("connections");
    assert( node );
    connectionElements = node->ToElement();
    assert( connectionElements  );

    //powi¹zanie modu³ów
    for( moduleXML = connectionElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {
         cout << (*moduleMap.find(moduleXML->Attribute("name1"))).second <<endl;
         cout << (*moduleMap.find(moduleXML->Attribute("name2"))).second <<endl;
         
         ConnectModules((*moduleMap.find(moduleXML->Attribute("name1"))).second, atoi(moduleXML -> Attribute("output")), (*moduleMap.find(moduleXML->Attribute("name2"))).second, atoi(moduleXML -> Attribute("input")));
    }
    
    TRACE("Algorithm", "Moduly polaczone");
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
