#include "desk.h"
#include "core_mp.h"
#include "xmlconfigfile.h"
#include "debug.h"



Module* DeskModule::GetRTSModule() {
    return rtsModule;    
}

void DeskModule::SetRTSModule(Module* rtsModule_) {
    rtsModule = rtsModule_;    
}

void DeskModule::SetModuleId(ModuleId moduleId_) {
    moduleId = moduleId_;    
}

//Konstruktor i destruktor -----------------------------------------------------
Desk::Desk(Algorithm *algo)
{
    algorithm           = algo;
    deskModuleActive    = NULL;
    AddAllModuleToDesk();
}

Desk::~Desk()
{
    delete algorithm;
}

//Obs�uga modu��w --------------------------------------------------------------
void Desk::AddModule(string type, string name)
{
    DeskModule *deskModule = new DeskModule(0,0);
    ModuleId moduleId = algorithm->AddModule(type,name);    
    deskModule->SetModuleId(moduleId);
    deskModule->SetRTSModule(algorithm->GetModule(moduleId));
    deskModules.push_back(deskModule);

}

void Desk::AddModule(ModuleId moduleId)
{
    TRACE("Desk::AddModule()","start");
    DeskModule *deskModule = new DeskModule(0,0);          
    deskModule->SetModuleId(moduleId);
    deskModule->SetRTSModule(algorithm->GetModule(moduleId));
    deskModules.push_back(deskModule);
    TRACE("Desk::AddModule()","end");

}

Module* Desk::FindModule(string name)
{
    if (algorithm->GetModule(name) != NULL)
        return algorithm->GetModule(name);
    return NULL;
}

int Desk::FindOutput(string nameModule,string outName)
{
    return FindOutput(FindModule(nameModule),outName);
}


int Desk::FindOutput(Module* module,string outName)
{
    if (module == NULL ) return -1;
    TRACE("Desk::FindOutput()","1");
    for (int i = 0; i < module->GetOutputCount(); i++)
    {
        TRACE("Desk::FindOutput()","*");
        if (module->GetOutput(i)->GetName() == outName)
            return module->GetOutput(i)->GetID();
    }
    return -1;
}

int Desk::FindInput(string nameModule,string inName)
{
    return FindInput(FindModule(nameModule),inName);
}

int Desk::FindInput(Module* module,string inName)
{
    if (module == NULL ) return -1;
    for (int i = 0; i < module->GetInputCount(); i++)
        if (module->GetInput(i)->GetName() == inName)
            return module->GetInput(i)->GetID();
    return -1;
}

void Desk::SetPosition(string nameModule,int x, int y)
{
    TRACE("Desk::SetPosition()","start");
    for (int i = 0;i<deskModules.size();i++)
        if (deskModules[i]->GetRTSModule() != NULL)
        {
            TRACE("Desk::SetPosition()","znaleziono");
            if (deskModules[i]->GetRTSModule()->GetName() == nameModule)
            {
                deskModules[i]->x=x;
                deskModules[i]->y=y;
            }
        }
    TRACE("Desk::SetPosition()","end");
}


DeskModule* Desk::GetDeskModuleActive()
{
    return deskModuleActive;    
}

void Desk::SetDeskModuleActive(DeskModule* deskModule)
{
    if (deskModuleActive != NULL)
        deskModuleActive->widget->SetSelected(false);    
    deskModuleActive = deskModule;        
    deskModuleActive->widget->SetSelected(true);
    deskModuleActive->widget->on_expose_event(NULL);
}


//Zapisywanie do plik ----------------------------------------------------------
void Desk::SaveToFile(string filename)
{
    //Zmienne do parsowania XML
	string xml =
	   "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
       "<algorithm>\n";

    //czy sa jakies moduly ??
    if (deskModules.size()>0)
    {
        xml+= "  <modules>\n";
        for (int i = 0;i<deskModules.size();i++)
        {
            if ((deskModules[i]->GetRTSModule()->GetType()!="audioportin")&(deskModules[i]->GetRTSModule()->GetType()!="audioportout"))
            {
                xml+= "    <module type=\""+ deskModules[i]->GetRTSModule()->GetType()+"\" name=\""+deskModules[i]->GetRTSModule()->GetName()+"\">\n";            
                if (deskModules[i]->GetRTSModule()->GetParameterCount()>0)
                    for (int p = 0;p<deskModules[i]->GetRTSModule()->GetParameterCount();p++) {
                        if ((deskModules[i]->GetRTSModule()->GetParameter(p)->GetGUIType() == gtProperty))
                        {
                            ParameterFloat* param =	(ParameterFloat*)deskModules[i]->GetRTSModule()->GetParameter(p);
                            xml+= "        <parameter number=\""+IntToString(p)+"\" type=\"float\">"+IntToString(param->GetValue())+"</parameter>";
                        }
                
                        if (deskModules[i]->GetRTSModule()->GetParameter(p)->GetGUIType() == gtParameter) {
                            ParameterString* param = (ParameterString*)deskModules[i]->GetRTSModule()->GetParameter(p);					
					       xml+= "        <parameter number=\""+IntToString(p)+"\" type=\"string\">"+param->GetText()+"</parameter>";
                        }               
                    }            
                xml+= "    </module>";
            }
        }
        xml+= "  </modules>\n";
    }
    
/*xx    xml+= "  <connections>\n";
    for (int c1=0; c1 < deskModules.size(); c1++)
    {
        for (int input = 0; input < deskModules[c1]->GetRTSModule()->GetInputCount(); input++)
        {          
            for (int c2=0; c2 < deskModules.size(); c2++)
            {
                if (deskModules[c2]->GetRTSModule()->GetID() == deskModules[c1]->GetRTSModule()->GetInput(input)->GetIDModule())
                {
                xml+= "    <connection name1=\"" + deskModules[c2]->GetRTSModule()->GetName() +
                              "\" output=\"" + IntToString(deskModules[c1]->GetRTSModule()->GetInput(input)->GetIDModuleOutput()) +
                               "\" name2=\"" + deskModules[c1]->GetRTSModule()->GetName() +
                               "\" input=\"" + IntToString(input) + "\"/> \n";
                }    
            }
        
        }    
    }
    xml+= "  </connections>\n";
*/
    //czy sa jakies moduly ??
    if (deskModules.size()>0)
    {
        xml+= "  <modules_widget>\n";
        for (int i = 0;i<deskModules.size();i++)
            xml+= "    <module_widget name=\"" + deskModules[i]->GetRTSModule()->GetName() +
                                      "\" x=\"" + IntToString(deskModules[i]->x) +
                                      "\" y=\"" + IntToString(deskModules[i]->y) + "\" />\n";
        xml+= "  </modules_widget>\n";
    }

    xml+= "</algorithm>\n";

    cout <<endl<< xml <<endl;
    TiXmlDocument doc(filename.c_str());

    doc.Parse( xml.c_str() );

    if ( doc.Error() )
	{
		printf( "Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );
		exit( 1 );
	}
	doc.SaveFile();
}


//------------------------------------------------------------------------------
void Desk::AddAllModuleToDesk() {
    bool eom = false;
    Module* mod = algorithm->GetFirstModule();
    while(!eom)
    {           
        AddModule(algorithm->GetModuleId(mod->GetName()));
        mod = algorithm->GetNextModule();  
        TRACE("Desk::LoadFromFile()", "*");
        if (mod==NULL) eom = true;
    }    
}

//Odczyt z pliku ---------------------------------------------------------------
void Desk::LoadFromFile(string filename)
{
    Clear();
    
    TRACE("Desk::LoadFromFile()", "load algorithm");
    XMLConfigFile xmlConfig;    
 	try {
        xmlConfig.OpenFile(filename.c_str());
		xmlConfig.LoadAlgorithm(algorithm);
    } catch (RTSError& error) {
        cout << "Error: " << error.what() << endl;
        exit(1);
    }
    
    TRACE("Desk::LoadFromFile()", "put module&widget on desk");
    
////////////////////////////////////////////////    
    for (int i = 0;i<deskModules.size();i++)
    {
         delete deskModules[i]->widget;
         delete deskModules[i]->text;         
    }   
    
    deskModules.clear(); 
    
    deskModuleActive = NULL;
////////////////////////////////////////////////    
    
    


    AddAllModuleToDesk();

    
    TRACE("Desk::LoadFromFile()", "load widget position");
    
    TiXmlDocument document;
    if ( !document.LoadFile(filename.c_str()) ) {
		throw RTSError("Nie mozna wczytac pliku " + (string)filename +
			"! Error: " + document.ErrorDesc());
    }
    
    TiXmlElement* moduleXMLElem;
	TiXmlNode* moduleXMLNode, * parent;
	string moduleName;
	int moduleId;


    TiXmlHandle docHandle( &document );
	parent = docHandle.FirstChild( "algorithm" ).FirstChild( "modules_widget" ).Child("module_widget", 0).Node();

	// przez wszystkie inne moduly
	if(parent != NULL) {
		for( moduleXMLNode = parent; moduleXMLNode; moduleXMLNode = moduleXMLNode->NextSibling("module_widget") ) {
			moduleXMLElem = moduleXMLNode->ToElement();
            SetPosition(moduleXMLElem->Attribute("name"),atoi(moduleXMLElem->Attribute("x")),atoi(moduleXMLElem->Attribute("y")));
            TRACE3("Pozycja",moduleXMLElem->Attribute("name"),atoi(moduleXMLElem->Attribute("x")),atoi(moduleXMLElem->Attribute("y")));
  		}
	}
	TRACE("Desk::LoadFromFile()", "end");

}


void Desk::Clear() {
    for (int i = 0;i<deskModules.size();i++)
    {
         delete deskModules[i]->widget;
         delete deskModules[i]->text;         
    }   
    
    deskModules.clear(); 
    
    deskModuleActive = NULL;
    
    algorithm->Clear();
    
    AddAllModuleToDesk();
}


void Desk::DeleteActiveModule() { 
    TRACE("Desk::DeleteActiveModule()", "start");   
    algorithm->DeleteModule(algorithm->GetModuleId(GetDeskModuleActive()->GetRTSModule()->GetName()));
    TRACE("Desk::DeleteActiveModule()", "1");
    
    vector<DeskModule*>::iterator iter;
    
    for (iter = deskModules.begin(); (*iter)!=deskModuleActive; iter++) {      
    }
    if ((*iter)==deskModuleActive) {
            TRACE("Desk::DeleteActiveModule()", "1.5");
            deskModules.erase(iter);
            TRACE("Desk::DeleteActiveModule()", "1.6");
        
    TRACE("Desk::DeleteActiveModule()", "2");
    delete deskModuleActive;
    deskModuleActive = NULL;
    }
    TRACE("Desk::DeleteActiveModule()", "end"); 
}
