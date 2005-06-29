#include <string>
#include <map>
#include <list>

#include <tinyxml.h>

#include "algorithmview.h"
#include "xmlconfigfile.h"

void AlgorithmView::LoadFromFile(string fileName) {
	XMLConfigFile xmlFile;
    std::map<string, GuiModule*> name2GuiModuleMap;
    
	Clear();

	xmlFile.OpenFile( fileName.c_str() );
	xmlFile.LoadAlgorithm(&algorithm);
	algorithm.Init();

	Module* mod;
	GuiModule* guiMod;
	for(ModuleIdIterator it = algorithm.ModuleIdIteratorBegin();
		it != algorithm.ModuleIdIteratorEnd(); it++)
	{
		mod = algorithm.GetModule(*it);
		guiMod = guiFactory.CreateGuiModule(mod);
		guiMod->SetParentView(this); // konieczne na razie :(
		name2GuiModuleMap.insert( make_pair(mod->GetName(), guiMod) );
		guiModules.push_back(guiMod);
	}
	
	TRACE("AlgorithmView::LoadFromFile()", "Wczytywanie polozenia modulow...");

	TiXmlElement* moduleXMLElem;
	TiXmlNode* moduleXMLNode, * parent;
	std::string moduleName;
	int x, y;
	ModuleId moduleId;

	TiXmlDocument document;
	document.LoadFile( fileName.c_str() );
    TiXmlHandle docHandle( &document );

	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "gui" ).Child("guimodule", 0).Node();

	// przez wszystkie inne moduly
	if(parent != NULL) {
		for( moduleXMLNode = parent; moduleXMLNode;
			moduleXMLNode = moduleXMLNode->NextSibling("guimodule") )
		{
			moduleXMLElem = moduleXMLNode->ToElement();
			moduleName = moduleXMLElem->Attribute("name");
			x = atoi( moduleXMLElem->Attribute("x") );
			y = atoi( moduleXMLElem->Attribute("y") );
			cout << "GUIIII: " << moduleName << "   " << x << "  " << y << endl;
			guiMod = ( *name2GuiModuleMap.find(moduleName) ).second;
			guiMod->SetXY(x, y);
  		}
	}

	TRACE("AlgorithmView::LoadFromFile()", "Polozenia modulow wczytane");

	for(std::list<GuiModule*>::iterator it = guiModules.begin();
		it != guiModules.end(); it++) {
		guiMod = *it;
		guiMod->GetPosition(x, y);
		this->put(*guiMod, x, y);
	}
	
	for(ConnectionIdIterator connIt = algorithm.ConnectionIdIteratorBegin();
	    connIt != algorithm.ConnectionIdIteratorEnd(); connIt++)
	{
		ConnectionId connId = *connIt;
		GuiConnection* guiConn = new GuiConnection;
		Connection* conn =  algorithm.GetConnection(connId);

		guiConn->Set(
			&connId,
			(*name2GuiModuleMap.find( conn->sourceModule->GetName() )).second,
			conn->sourceOutputId,
			(*name2GuiModuleMap.find( conn->destinationModule->GetName() )).second,
			conn->destinationInputId
		);

  		connections.push_back(guiConn);
	}

	show_all_children();
}
