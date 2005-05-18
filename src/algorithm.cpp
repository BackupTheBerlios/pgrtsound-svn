#include "algorithm.h"

Algorithm::Algorithm() {
	// wyjscie audio jest zawsze
	AudioPortOut* apo = new AudioPortOut;
	apo->SetID(0);
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
	i--; // cofamy o jeden do ostatniegom modulu - powinien byc AudioPortOut
	return modulesQueue[i]->GetInput(0)->GetSignal();
}

int Algorithm::AddModule(string type) {
	Module* m = moduleFactory.CreateModule(type);
	
	m->SetID( modules.size() );
	modules.push_back(m);

	TRACE3("Algorithm", "Modul '", type, "' dodany do algorytmu");
	
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
			modules[moduleId1]->GetType() << "(" << modules[moduleId1]->GetOutput(outputId)->GetName() << ") -> " <<
			modules[moduleId2]->GetType() << "(" << modules[moduleId2]->GetInput(inputId)->GetName() << ")" << endl;
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
    assert( moduleElements );

    // stworzenie wszystkich modu³ów
    //pierw g³oœnika
    moduleName2IDMap.insert(make_pair("0",0));
    //a potem reszte :)
    for( moduleXML = moduleElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {    
       moduleName2IDMap.insert(make_pair(moduleXML->Attribute("name"),AddModule(moduleXML->Attribute("type"))));
	}
	
	TRACE("Algorithm", "Moduly wczytane");
}

void Algorithm::LoadParametersFromFile(const char * filename) {
	TRACE("Algorithm", "Wczytywanie parametrow...");

	TiXmlDocument doc(filename);
	bool loadOkay = doc.LoadFile();

    if ( !loadOkay ) {
        throw RTSError("Nie mo¿na wczytaæ pliku " + (string)filename + " Error: " + doc.ErrorDesc());
    }

	string paramValue, paramType;
  	int paramId, moduleId;
	TiXmlHandle docHandle( &doc );
	TiXmlElement* moduleElem;
	TiXmlElement* paramElem;
	TiXmlNode* moduleNode;
	TiXmlNode* parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules" ).Child("module", 1).Node();

	// wszystkie moduly
	for( moduleNode = parent; moduleNode; moduleNode = moduleNode->NextSibling("module") ) {
		moduleElem = moduleNode->ToElement();
		moduleId = (*moduleName2IDMap.find(moduleElem->Attribute("name"))).second;

		// wszystkie parametry kazdego modulu
		for( paramElem = moduleNode->FirstChildElement("parameter"); paramElem; paramElem = moduleNode->NextSiblingElement("parameter") ) {
			if(paramElem != NULL) {
                TiXmlNode* txt = paramElem->FirstChild();
                
                paramType = paramElem->Attribute("type");
				paramId = atoi(paramElem->Attribute("number"));
				if(txt != NULL) {
					paramValue = txt->Value();
				}
				
    			#ifndef NDEBUG
				cout << "     " << moduleElem->Attribute("name") << "." <<
					modules[moduleId]->GetParameter(paramId)->GetName() << " = ";
				#endif

				if(paramType == "float") {
					float value;
					ParameterFloat* param = (ParameterFloat*)modules[moduleId]->GetParameter(paramId);
					
					value = atof(paramValue.c_str());
					param->SetValue(value);
					#ifndef NDEBUG
						cout << param->GetValue() << endl;
					#endif
				}
				
				if(paramType == "string") {
					ParameterString* param = (ParameterString*)modules[moduleId]->GetParameter(paramId);
					param->SetText(paramValue);
					#ifndef NDEBUG
					    cout << param->GetText() << endl;
				    #endif
				}
				
			}
		}
	}
//(ParameterFloat*)modules[ (*moduleName2IDMap.find(moduleXML->Attribute("name"))).second ]

}

///**
// * Funckja wczytuj¹ca do pamiêci parametry modu³ów
// * @param Nazwa plik zaierajacego Algorytm
//*/
//void Algorithm::LoadParametersFromFile(const char * filename) {
//    TRACE("Algorithm", "Wczytywanie parametrow...");
//
//    TiXmlNode*    node                  = 0;
//    TiXmlElement* projectElement        = 0;
//    TiXmlElement* moduleElements        = 0;
//    TiXmlElement* moduleXML             = 0;
//
//    TiXmlDocument doc(filename);
//    bool loadOkay = doc.LoadFile();
//
//    if ( !loadOkay )
//    {
//        //cout << "Could not load file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
//        //exit( 1 );
//        throw RTSError("Nie mo¿na wczytaæ pliku " + (string)filename + " Error: " + doc.ErrorDesc());
//    }
//
//    node = doc.FirstChild( "algorithm" );
//    assert( node );
//    projectElement = node->ToElement();
//    assert( projectElement );
//
//    node = projectElement->FirstChildElement("parameters");
//    assert( node );
//    moduleElements = node->ToElement();
//    assert( moduleElements  );
//
//    for( moduleXML = moduleElements->FirstChildElement();
//         moduleXML;
//         moduleXML = moduleXML->NextSiblingElement() )
//    {
//        //modules[ (*moduleName2IDMap.find(moduleXML->Attribute("name"))).second ]->SetParam(atoi(moduleXML -> Attribute("number")), atof(moduleXML -> Attribute("value")));
//		( (ParameterFloat*)modules[ (*moduleName2IDMap.find(moduleXML->Attribute("name"))).second ]->GetParameter(atoi(moduleXML -> Attribute("number")))  )->SetValue( atof(moduleXML -> Attribute("value")) );
//    }
//
// 	TRACE("Algorithm", "Parametry wczytane");
//}

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
         //cout << (*moduleMap.find(moduleXML->Attribute("name1"))).second <<endl;
         //cout << (*moduleMap.find(moduleXML->Attribute("name2"))).second <<endl;
         
         ConnectModules(
		 	(*moduleName2IDMap.find(moduleXML->Attribute("name1"))).second,
		 	atoi(moduleXML -> Attribute("output")),
			(*moduleName2IDMap.find(moduleXML->Attribute("name2"))).second,
			atoi(moduleXML -> Attribute("input")));
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
