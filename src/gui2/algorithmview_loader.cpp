#include "algorithmview.h"
#include "./../xmlconfigfile.h"

#include <tinyxml.h>

#include <string>
#include <map>
#include <list>

void AlgorithmView::LoadFromFile(string fileName) {
	XMLConfigFile xmlFile;

	window->freeze_updates();

	Clear();

	xmlFile.OpenFile( fileName.c_str() );
	xmlFile.LoadAlgorithm(&algorithm);
	algorithm.Init();

	// utworzenie modulow
	Module* mod;
	GuiModule* guiMod;
	for( ModuleIdIterator it = algorithm.ModuleIdIteratorBegin();
		it != algorithm.ModuleIdIteratorEnd(); it++ )
	{
		mod = algorithm.GetModule( *it );
		guiMod = guiFactory.CreateGuiModule( mod );
		guiMod->SetParentView(this); // konieczne na razie :(
		guiMod->SetModuleId( algorithm.GetModuleId( mod->GetName() ) );
		name2GuiModuleMap.insert( make_pair(mod->GetName(), guiMod) );
		guiModules.push_back( guiMod );
	}
	
	TRACE("AlgorithmView::LoadFromFile()", "Wczytywanie polozenia modulow...");

	TiXmlElement* moduleXMLElem;
	TiXmlNode* moduleXMLNode, * parent;
	std::string moduleName;
	int x, y;
	//ModuleId moduleId;

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
			cout << "GUI: " << moduleName << "   " << x << "  " << y << endl;
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
	
	// utworzenie GUI-polaczen na podstawie polaczen w obiekcie Algorithm
	for(ConnectionIdIterator connIt = algorithm.ConnectionIdIteratorBegin();
	    connIt != algorithm.ConnectionIdIteratorEnd(); connIt++)
	{
		ConnectionId connId = *connIt;
		GuiConnection* guiConn = new GuiConnection;
		Connection* conn =  algorithm.GetConnection(connId);

		guiConn->Set(
			connId,
			(*name2GuiModuleMap.find( conn->sourceModule->GetName() )).second,
			conn->sourceOutputId,
			(*name2GuiModuleMap.find( conn->destinationModule->GetName() )).second,
			conn->destinationInputId
		);
		
		(*name2GuiModuleMap.find( conn->destinationModule->GetName() )).second
			->SetInputGuiConnection( conn->destinationInputId, guiConn );
			
  		connections.push_back(guiConn);
	}

	window->thaw_updates();

	show_all_children();
}
