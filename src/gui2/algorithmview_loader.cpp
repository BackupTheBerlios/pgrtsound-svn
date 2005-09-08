#include "algorithmview.h"
#include "./../xmlconfigfile.h"

#include <tinyxml.h>

#include <string>
#include <map>
#include <list>
#include <fstream>

void AlgorithmView::LoadFromFile(string fileName) {
	XMLConfigFile xmlFile;

	window->freeze_updates();

	Clear();

	xmlFile.OpenFile( fileName.c_str() );
	xmlFile.LoadAlgorithm( &algorithm );

	// utworzenie GUI modulow
	Module* mod;
	GuiModule* guiMod;
	for( ModuleIdIterator it = algorithm.ModuleIdIteratorBegin();
		it != algorithm.ModuleIdIteratorEnd(); it++ )
	{
		mod = algorithm.GetModule( *it );

		//if( mod->GetName() == "AudioPortIn" || mod->GetName() == "AudioPortOut" ) {
		//	cout << "    pomijam " << mod->GetName() << endl;
		// continue;
		//}
		
		guiMod = guiFactory.CreateGuiModule( mod );
		guiMod->SetParentView( this ); // konieczne na razie :(
		//guiMod->SetModuleId( algorithm.GetModuleId( mod->GetName() ) );
		guiMod->SetModuleId( *it );
		name2GuiModuleMap.insert( make_pair(mod->GetName(), guiMod) );
		guiModules.push_back( guiMod );
	}
	
	TRACE("AlgorithmView::LoadFromFile - Wczytywanie polozenia modulow...\n");

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

	TRACE("AlgorithmView::LoadFromFile - Polozenia modulow wczytane\n");

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
	
	TRACE("AlgorithmView::LoadFromFile - Polaczenia modulow wczytane\n");
}

/*
 Zapis do pliku.
*/
void AlgorithmView::SaveToFile( string fileName ) {
	cout << "save " << fileName << endl;

    std::ofstream file;
    file.open( fileName.c_str(), ios::out );
    
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
	file <<	"<algorithm>\n";
	file << "\t<modules>\n";
	
	for( list<GuiModule*>::iterator modIt = guiModules.begin();
		modIt != guiModules.end(); modIt++ )
	{
		Module* mod = (*modIt)->GetModule();
		
		if( mod->GetName() == "AudioPortIn" || mod->GetName() == "AudioPortOut")
		    continue;
		
		file << "\t\t<module type=\"" << mod->GetType()
			<< "\" name=\"" << mod->GetName() << "\">\n";

		for( int i = 0; i < mod->GetParameterCount(); i++ ) {
//           	file << "\t\t\t<parameter name=\"" << mod->GetParameter( i )->GetName()
//			   << "\" number=\"" << i << "\">";

           	file << "\t\t\t<parameter number=\"" << i << "\">";

			if( mod->GetParameter( i )->GetType() == "float" ) {
			    ParameterFloat* param = (ParameterFloat*)mod->GetParameter( i );
			    file << param->GetValue();
			}
			    
   			if( mod->GetParameter( i )->GetType() == "string" ) {
			    ParameterString* param = (ParameterString*)mod->GetParameter( i );
			    file << param->GetText();
			}
			
		   file << "</parameter>\n";
		}
		file << "\t\t</module>\n";
	}

	file << "\t</modules>\n";

	file << "\t<connections>\n";
	
	for(list<GuiConnection*>::iterator connIt = connections.begin();
		connIt != connections.end(); connIt++)
	{
		Connection* conn = algorithm.GetConnection( (*connIt)->GetConnectionId() );

		file << "\t\t<connection name1=\""
			<< conn->sourceModule->GetName() << "\" output=\""
			<< conn->sourceOutputId << "\" "
			<< "name2=\""
			<< conn->destinationModule->GetName() << "\" input=\""
			<< conn->destinationInputId << "\" />\n";
	}

	file <<	"\t</connections>\n";
	file <<	"\t<gui>\n";

	int x, y;
	for( list<GuiModule*>::iterator modIt = guiModules.begin();
		modIt != guiModules.end(); modIt++ )
	{
		(*modIt)->GetPosition( x, y );
		file << "\t\t<guimodule name=\"" << (*modIt)->GetModule()->GetName()
			<< "\" x=\"" << x << "\" y=\"" << y << "\" />\n";
	}

	file << "\t</gui>\n";
	file <<	"</algorithm>\n";

	file.close();
}
