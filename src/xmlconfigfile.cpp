#include "xmlconfigfile.h"

XMLConfigFile::XMLConfigFile() {
}

XMLConfigFile::~XMLConfigFile() {
	TRACE( "XMLConfigFile::~XMLConfigFile - Destrukcja\n" );
}

void XMLConfigFile::OpenFile(const char * filename) {
	fileName = filename;

    if ( !document.LoadFile(fileName.c_str()) ) {
		throw RTSError("Nie mozna wczytac pliku " + (string)fileName +
			"! Error: " + document.ErrorDesc());
    }
    
}

/**
 Wczytuje atrybuty algorytmu.
 Wczytana zostaje miedzy innymi nazwa alogrytmu.
 
 @param algo Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadAlgorithmSettings(Algorithm* algo) {
    TRACE( "XMLConfigFile::LoadAlgorithmSettings - Wczytywanie ustawien algorytmu...\n" );
	TiXmlElement* algoXmlElem;
	TiXmlNode* algoXmlNode;
	string algoName;

    TiXmlHandle docHandle( &document );
	algoXmlNode = docHandle.FirstChild( "algorithm" ).Node();
	if(algoXmlElem != NULL) {
       	algoXmlElem = algoXmlNode->ToElement();
		if( algoXmlElem->Attribute("name") )
			algo->SetName( algoXmlElem->Attribute("name") );
	}
}

/**
 Wczytuje algorytm opisany w pliku
 @param algo Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadAlgorithm(Algorithm* algo) {
	TRACE("XMLConfigFile::LoadModules - Wczytywanie calego algorytmu...\n" );
	LoadAlgorithmSettings(algo);
	LoadModules(algo);
	LoadParameters(algo);
	LoadConnections(algo);
	algo->Init();
	TRACE( "XMLConfigFile::LoadModules - Algorytm wczytany\n" );
}

/**
 * Wczytuje modu³y zadeklarowane w pliku
  * @param algo Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadModules(Algorithm* algo) {
    TRACE( "XMLConfigFile::LoadModules - Wczytywanie modulow...\n" );

	TiXmlElement* moduleXMLElem;
	TiXmlNode* moduleXMLNode, * parent;
	string moduleName;
	//int moduleId;
	ModuleId moduleId;

    TiXmlHandle docHandle( &document );
	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules" ).Child("module", 0).Node();

	// przez wszystkie inne moduly
	if(parent != NULL) {
		for( moduleXMLNode = parent; moduleXMLNode; moduleXMLNode = moduleXMLNode->NextSibling("module") ) {
			moduleXMLElem = moduleXMLNode->ToElement();
			moduleName = moduleXMLElem->Attribute("name");
			
			try {
		  	   moduleId = algo->AddModule( moduleXMLElem->Attribute("type"), moduleName );
            }
            catch ( RTSError x ) {
                TRACE( "%s\n", x.what() );
            }
  		}
	}

	TRACE( "XMLConfigFile::LoadModules - Moduly wczytane\n" );
}

/**
 * Funckja wczytuj¹ca parametry modu³ów
 * @param algo Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadParameters(Algorithm* algo) {
	TRACE("XMLConfigFile::LoadParameters - Wczytywanie parametrow...\n" );

	string paramValue, paramType, moduleName;
  	int paramId;
	TiXmlElement* moduleElem, * paramElem;
	TiXmlNode* moduleNode, * parent, * paramTxt;

    TiXmlHandle docHandle( &document );
	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules" ).Child("module", 0).Node();

	// wszystkie moduly
	for( moduleNode = parent; moduleNode; moduleNode = moduleNode->NextSibling("module") ) {
		moduleElem = moduleNode->ToElement();
		//moduleId = (*moduleName2IdMap.find(moduleElem->Attribute("name"))).second;
		moduleName = moduleElem->Attribute("name");

		// wszystkie parametry kazdego modulu
		for( paramElem = moduleNode->FirstChildElement("parameter");
			paramElem;
			paramElem = paramElem->NextSiblingElement("parameter") )
		{
			if(paramElem != NULL) {
                paramTxt = paramElem->FirstChild();
				paramId = atoi(paramElem->Attribute("number"));
                paramType = algo->GetModule(moduleName)->GetParameter(paramId)->GetType();

				if(paramTxt != NULL) {
					paramValue = paramTxt->Value();
				}
    			#ifndef NDEBUG
				cout << "    " << moduleElem->Attribute("name") << "." <<
					algo->GetModule(moduleName)->GetParameter(paramId)->GetName() << " = ";
				#endif

				if(paramType == "float") {
					float value;
					ParameterFloat* param =
						(ParameterFloat*)algo->GetModule(moduleName)->GetParameter(paramId);
					value = (float)atof( paramValue.c_str() );
					param->SetValue(value);
					#ifndef NDEBUG
						cout << param->GetValue() << endl;
					#endif
				}

				if(paramType == "string") {
					ParameterString* param =
						(ParameterString*)algo->GetModule(moduleName)->GetParameter(paramId);
					param->SetText(paramValue);
					#ifndef NDEBUG
					    cout << param->GetText() << endl;
				    #endif
				}
			}
		}
	}
	
	TRACE( "XMLConfigFile::LoadParameters - Parametry wczytane\n" );
}

/**
 * Funckja wczytuj¹ca polaczenia miêdzy modu³ami
 * @param algo Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadConnections(Algorithm* algo) {
    TRACE( "XMLConfigFile::LoadConnections - Wczytywanie polaczen...\n" );

	string module1Name, module2Name;
  	int inputId = -1, outputId = -1;
	TiXmlElement* connElem;
	TiXmlNode* connNode, * parent;

	TiXmlHandle docHandle( &document );
	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "connections" ).Child("connection", 0).Node();

	for( connNode = parent; connNode; connNode = connNode->NextSibling("connection") ) {
		connElem = connNode->ToElement();

		module1Name = connElem->Attribute("name1");
		module2Name = connElem->Attribute("name2");

		inputId = atoi( connElem->Attribute("input") );
		outputId = atoi( connElem->Attribute("output") );

		ConnectionId tempId;
		algo->ConnectModules(module1Name, outputId, module2Name, inputId, tempId);
	}

    TRACE( "XMLConfigFile::LoadConnections - Moduly polaczone\n");
}


