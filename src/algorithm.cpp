#include "algorithm.h"

Algorithm::Algorithm() {
	// wyjscie audio jest zawsze
	AudioPortOut* apo = new AudioPortOut;
	apo->id = 0;
	modules.push_back(apo);	
}

Algorithm::~Algorithm() { 
    for (int i=0; i<=modules.size(); i++)
    {
        delete adjacencyMatrix[i];
      	delete modules[i];
    } 
    delete adjacencyMatrix;
    

	
	cout << "Bye.";
}

/**
 * Uruchomienie przetwarzania przez algorytm.
 * Wywolanie tej funkcji spowoduje przeliczenie bloku ramek o dlugosci
 * framesPerBlock.
 */
float* Algorithm::process() {
	int i;
	for(i = 0; i < modulesQueue.size(); i++) {
		modulesQueue[i]->process();
	}
	i--;
	return modulesQueue[i]->input(0).signal;
}

int Algorithm::addModule(string type) {
	Module* m = moduleFactory.createModule(type);
	
	m->id = modules.size();

	//for(int n=0; n < 10; n++) m->process();
	modules.push_back(m);

	#ifndef NDEBUG
		cout << "Modul '" << type << "' dodany do algorytmu" << endl;
	#endif
	
	return m->id;
}

void Algorithm::printInfo() {
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

Module* Algorithm::module(int moduleId) {
	return modules[moduleId];
}

/**
 * Laczenie dwoch blokow
 */
void Algorithm::connectModules(int moduleId1, int outputId, int moduleId2, int inputId) {
	#ifndef NDEBUG
		cout << "Lacze " << 
			module(moduleId1)->type << "(" << module(moduleId1)->output(outputId).name << ") -> " <<
			module(moduleId2)->type << "(" << module(moduleId2)->input(inputId).name << ")" << endl;
			cout << "Lacze " << 
			moduleId1 << "(" << module(moduleId1)->output(outputId).id << ") -> " <<
			moduleId2 << "(" << module(moduleId2)->input(inputId).id << ")" << endl;
	#endif
 	module(moduleId2)->input(inputId) = module(moduleId1)->output(outputId);

    adjacencyMatrix[moduleId1][moduleId2] = 1;
    adjacencyMatrix[moduleId2][moduleId1] = -1;
}



/**
 * Dla celow testowych mozna za pomoca tej funckji ustawci recznie kolejnosc
 * przetwarzania modulow.
 */
void Algorithm::setQueueManually(int* order, int num) {
	modulesQueue.clear();
	for(int i = 0; i < num; i++) {
		modulesQueue.push_back( modules[ order[i] ] );
	}
}

void Algorithm::setFramesPerBlock(unsigned long fpb) {
	framesPerBlock = fpb;
	Module::framesPerBlock = fpb; // wartos widoczna we wszystkich modulach
	#ifndef NDEBUG
		cout << "Module::framesPerBlock = " << Module::framesPerBlock << endl;
	#endif

/* TODO (#1#): reinicjalizacja buforow wyjsciowych wszystkich 
               modulow */	
}

void Algorithm::setSampleRate(int sRate) {
	sampleRate = sRate;
	Module::sampleRate = sRate; // wartos widoczna we wszystkich modulach
	#ifndef NDEBUG
		cout << "Module::sampleRate = " << Module::sampleRate << endl;
	#endif
 
/* TODO (#1#): trzeba dac znac modulom ze sie zmienilo  */	

}


/**
 * Funckja wczytuj¹ca do pamiêci modu³y 
 * @param Nazwa plik zaierajacego Algorytm
*/
void Algorithm::LoadModulesFromFile(const char * filename) {       
    int module;
      
    //Zmienne do parsowania XML
    int moduleId;
    int modulesCount  = 0;
    
    
    string moduleType;
    string moduleName;
    string strTemp;
    TiXmlNode* node                     = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* connectionElements    = 0;
    TiXmlElement* moduleElements        = 0;
    TiXmlElement* element               = 0;
    TiXmlElement* moduleXML             = 0;
    
    
    
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
        cout << "Could not load file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
        exit( 1 );
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
    for( moduleXML = moduleElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {
      
       moduleType = moduleXML->Attribute("type");
       //moduleName = moduleXML->Attribute("name");      
       moduleId   = atoi(moduleXML->Attribute("id"));  
       module     = addModule(moduleType);
       if (module != moduleId) 
            exit(1);

     }
        
}

/**
 * Funckja wczytuj¹ca do pamiêci parametry modu³ów
 * @param Nazwa plik zaierajacego Algorytm
*/
void Algorithm::LoadParametersFromFile(const char * filename) {                          
    //Zmienne do parsowania XML
    int moduleId;
    int modulesCount  = 0;
        
    string moduleType;
    string moduleName;
    string strTemp;
    TiXmlNode* node                     = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* connectionElements    = 0;
    TiXmlElement* moduleElements        = 0;
    TiXmlElement* element               = 0;
    TiXmlElement* moduleXML             = 0;        
    
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
        cout << "Could not load file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
        exit( 1 );
    } 

    node = doc.FirstChild( "algorithm" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );
   
   
    node = projectElement->FirstChildElement("parameters");
    assert( node );
    moduleElements = node->ToElement();
    assert( moduleElements  );

              
    //powi¹zanie modu³ów
  
    for( moduleXML = moduleElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {
        module(atoi(moduleXML -> Attribute("idModule")))->setParam(atoi(moduleXML -> Attribute("number")), atoi(moduleXML -> Attribute("value")));         
    }
}

/**
 * Funckja wczytuj¹ca do pamiêci powi¹zania miêdzy modu³ami 
 * @param Nazwa plik zaierajacego Algorytm
*/
void Algorithm::LoadConnectionsFromFile(const char * filename) {       
    //Zmienne do parsowania XML
    int moduleId;
    int modulesCount  = 0;
    
    
    string moduleType;
    string moduleName;
    string strTemp;
    TiXmlNode* node                     = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* connectionElements    = 0;
    TiXmlElement* moduleElements        = 0;
    TiXmlElement* element               = 0;
    TiXmlElement* moduleXML             = 0;
    
    
    
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
        cout << "Could not load file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
        exit( 1 );
    } 

    node = doc.FirstChild( "algorithm" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );
   
   
    node = projectElement->FirstChildElement("connections");
    assert( node );
    moduleElements = node->ToElement();
    assert( moduleElements  );

              
    //powi¹zanie modu³ów
  
    for( moduleXML = moduleElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {
         connectModules(atoi(moduleXML -> Attribute("idModule1")), atoi(moduleXML -> Attribute("inlet")), atoi(moduleXML -> Attribute("idModule2")), atoi(moduleXML -> Attribute("outlet")));    
    }
}

/**
 * Funckja tworz¹ca macierz s¹siedztwa
*/
void Algorithm::CreateAdjacencyMatrix(void) {
    //Tworzenie macierzy s¹siedztwa
    adjacencyMatrix = new int*[modules.size()];
    for (int i=0; i<=modules.size(); i++)
    {
        adjacencyMatrix[i]  = new int[modules.size()];
    } 
    
    for(int i = 0; i < modules.size(); i++) {
        for(int j = 0; j < modules.size(); j++) {
		  adjacencyMatrix[i][j] = 0;
        }
	}
}

 
void Algorithm::CreateQueue(void) {
    
    int edgeCount = 0;
    int edgeIndex = 0;
    for(int i = 0; i < modules.size(); i++) {
        for(int j = i; j < modules.size(); j++) {
		  if (adjacencyMatrix[i][j] !=0) edgeCount++;         
        }
	} 
	
	cout << "Liczba krawedzi = " << edgeCount << endl;
   	
    using namespace boost;

    typedef adjacency_list<vecS, vecS, directedS,property<vertex_color_t, default_color_type> > Graph;
        
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    
    
    
    Pair edges[edgeCount];
    
    edgeCount = 0;
    for(int i = 0; i < modules.size(); i++) {
        for(int j = i; j < modules.size(); j++) {
		  if (adjacencyMatrix[i][j] ==1)
		  {          
              cout << "Pair(" << i << "," << j << ")" << endl;
		      edges[edgeIndex] = Pair (i,j);
		      edgeIndex++;
          }
          if (adjacencyMatrix[i][j] ==-1)
		  {
              cout << "Pair(" << j << "," << i << ")" << endl;
		      edges[edgeIndex] = Pair (j,i);
		      edgeIndex++;
          }          
        }        
	} 
	
    Graph G(5);
    for (std::size_t j = 0; j < 6; ++j)
        add_edge(edges[j].first, edges[j].second, G);
    
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
}
