// Class automatically generated by Dev-C++ New Class wizard

#include "xmlconfigfile.h" // class's header file

XMLConfigFile::XMLConfigFile() {
}

XMLConfigFile::~XMLConfigFile() {
}

void XMLConfigFile::OpenFile(const char * filename) {
	fileName = filename;

    if ( !document.LoadFile(fileName.c_str()) ) {
        throw RTSError("Nie mozna wczytac pliku " + (string)filename +
			"! Error: " + document.ErrorDesc());
    }
}

void XMLConfigFile::LoadAlgorithm(Algorithm* algo) {
}

/**
 * Wczytuje modu�y zadeklarowane w pliku
  * @param Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadModules(Algorithm* algo) {

    TRACE("XMLConfigFile::LoadModules()", "Wczytywanie modulow...");

	TiXmlElement* moduleXMLElem;
	TiXmlNode* moduleXMLNode, * parent;
	string moduleName;
	int moduleId;

    // zmapowanie modulow specjlanych
	moduleName2IdMap.insert(make_pair("AudioPortIn", 0));
	moduleName2IdMap.insert(make_pair("AudioPortOut", 1));

    TiXmlHandle docHandle( &document );
	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules" ).Child("module", 0).Node();

	// przez wszystkie inne moduly
	if(parent != NULL) {
		for( moduleXMLNode = parent; moduleXMLNode; moduleXMLNode = moduleXMLNode->NextSibling("module") ) {
			moduleXMLElem = moduleXMLNode->ToElement();
			moduleName = moduleXMLElem->Attribute("name");
			moduleId = algo->AddModule(moduleXMLElem->Attribute("type"));
			algo->GetModule(moduleId)->SetName(moduleName);
			moduleName2IdMap.insert(make_pair(moduleName, moduleId));
  		}
	}

	TRACE("XMLConfigFile::LoadModules()", "Moduly wczytane");
}

/**
 * Funckja wczytuj�ca parametry modu��w
 * @param Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadParameters(Algorithm* algo) {
	TRACE("Algorithm", "Wczytywanie parametrow...");

	string paramValue, paramType;
  	int paramId, moduleId;
	TiXmlElement* moduleElem, * paramElem;
	TiXmlNode* moduleNode, * parent, * paramTxt;

    TiXmlHandle docHandle( &document );
	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules" ).Child("module", 0).Node();

	cout << "PARENT: " << parent << endl;

	// wszystkie moduly
	for( moduleNode = parent; moduleNode; moduleNode = moduleNode->NextSibling("module") ) {
		moduleElem = moduleNode->ToElement();
		moduleId = (*moduleName2IdMap.find(moduleElem->Attribute("name"))).second;

		// wszystkie parametry kazdego modulu
		for( paramElem = moduleNode->FirstChildElement("parameter");
			paramElem;
			paramElem = moduleNode->NextSiblingElement("parameter") )
		{
			if(paramElem != NULL) {
                paramTxt = paramElem->FirstChild();
                paramType = paramElem->Attribute("type");
				paramId = atoi(paramElem->Attribute("number"));

				if(paramTxt != NULL) {
					paramValue = paramTxt->Value();
				}
    			#ifndef NDEBUG
				cout << "     " << moduleElem->Attribute("name") << "." <<
					algo->GetModule(moduleId)->GetParameter(paramId)->GetName() << " = ";
				#endif

				if(paramType == "float") {
					float value;
					ParameterFloat* param =
						(ParameterFloat*)algo->GetModule(moduleId)->GetParameter(paramId);
					value = atof(paramValue.c_str());
					param->SetValue(value);
					#ifndef NDEBUG
						cout << param->GetValue() << endl;
					#endif
				}

				if(paramType == "string") {
					ParameterString* param =
						(ParameterString*)algo->GetModule(moduleId)->GetParameter(paramId);
					param->SetText(paramValue);
					#ifndef NDEBUG
					    cout << param->GetText() << endl;
				    #endif
				}
			}
		}
	}
}

/**
 * Funckja wczytuj�ca do pami�ci powi�zania mi�dzy modu�ami
 * @param Wskaznik do konfigurowanego algorytmu
*/
void XMLConfigFile::LoadConnections(Algorithm* algo) {
    TRACE("Algorithm", "Wczytywanie polaczen...");

    string strTemp;
    //Zmienne do parsowania XML
    TiXmlNode*    node                  = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* connectionElements    = 0;
    TiXmlElement* moduleXML             = 0;

    node = document.FirstChild( "algorithm" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );

	node = projectElement->FirstChildElement("connections");
    assert( node );
    connectionElements = node->ToElement();
    assert( connectionElements  );

    //powi�zanie modu��w
    for( moduleXML = connectionElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {
         //cout << (*moduleName2IDMap.find(moduleXML->Attribute("name1"))).second <<endl;
         //cout << (*moduleName2IDMap.find(moduleXML->Attribute("name2"))).second <<endl;

		algo->ConnectModules(
		 	(*moduleName2IdMap.find(moduleXML->Attribute("name1"))).second,
		 	atoi(moduleXML -> Attribute("output")),
			(*moduleName2IdMap.find(moduleXML->Attribute("name2"))).second,
			atoi(moduleXML -> Attribute("input")));
	    }

    TRACE("Algorithm", "Moduly polaczone");
}