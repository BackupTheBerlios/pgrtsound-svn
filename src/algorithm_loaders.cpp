#include "algorithm.h"

void Algorithm::LoadAlgorithmSettingsFile(const char * filename) {
}

/**
 * Funckja wczytuj¹ca do pamiêci modu³y
 * @param Nazwa plik zaierajacego Algorytm
*/
void Algorithm::LoadModulesFromFile(const char * filename) {
    TRACE("Algorithm", "Wczytywanie modulow...");

	TiXmlElement* moduleXMLElem;
	TiXmlNode* moduleXMLNode, * parent;
	string moduleName;
	int moduleId;

    TiXmlDocument doc(filename);
    TiXmlHandle docHandle( &doc );

    bool loadOkay = doc.LoadFile();
    if ( !loadOkay ) {
        throw RTSError("Nie mo¿na wczytaæ pliku " + (string)filename + " Error: " + doc.ErrorDesc());
    }

    // zmapowanie modulow specjlanych
	moduleName2IDMap.insert(make_pair("AudioPortIn", 0));
	moduleName2IDMap.insert(make_pair("AudioPortOut", 1));

	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules" ).Child("module", 0).Node();

	// przez wszystkie inne moduly
	if(parent != NULL) {
		for( moduleXMLNode = parent; moduleXMLNode; moduleXMLNode = moduleXMLNode->NextSibling("module") ) {
			moduleXMLElem = moduleXMLNode->ToElement();
			moduleName = moduleXMLElem->Attribute("name");
			moduleId = AddModule(moduleXMLElem->Attribute("type"));
			modules[moduleId]->SetName(moduleName);
			moduleName2IDMap.insert(make_pair(moduleName, moduleId));
  		}
	}

	TRACE("Algorithm", "Moduly wczytane");
}

/**
 * Funckja wczytuj¹ca parametry modu³ów
 * @param Nazwa plik zaierajacego Algorytm
*/
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
	TiXmlElement* moduleElem, * paramElem;
	TiXmlNode* moduleNode, * parent, * paramTxt;

	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules" ).Child("module", 0).Node();

	cout << "PARENT: " << parent << endl;

	// wszystkie moduly
	for( moduleNode = parent; moduleNode; moduleNode = moduleNode->NextSibling("module") ) {
		moduleElem = moduleNode->ToElement();
		moduleId = (*moduleName2IDMap.find(moduleElem->Attribute("name"))).second;

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
		} // eof parametry
	} // eof moduly
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

    if ( !doc.LoadFile() ) {
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
         //cout << (*moduleName2IDMap.find(moduleXML->Attribute("name1"))).second <<endl;
         //cout << (*moduleName2IDMap.find(moduleXML->Attribute("name2"))).second <<endl;

         ConnectModules(
		 	(*moduleName2IDMap.find(moduleXML->Attribute("name1"))).second,
		 	atoi(moduleXML -> Attribute("output")),
			(*moduleName2IDMap.find(moduleXML->Attribute("name2"))).second,
			atoi(moduleXML -> Attribute("input")));
	    }

    TRACE("Algorithm", "Moduly polaczone");
}
