#include "desk.h"
#include "core_mp.h"
#include "xmlconfigfile.h"
#include "debug.h"


//Konstruktor i destruktor -----------------------------------------------------
Desk::Desk(Algorithm *algo) {
    algorithm           = algo;
    deskModuleActive    = NULL;
    AddAllModuleToDesk();
}

Desk::~Desk() {
    delete algorithm;
}

//Obs³uga modu³ów --------------------------------------------------------------
void Desk::AddModule(string type, string name) {
    //pierw dodanie modulu do algorytmu
    ModuleId moduleId = algorithm->AddModule(type,name);    
    //a teraz tworzenie GuiModule
    AddModule(moduleId);

}


void Desk::AddModule(ModuleId moduleId) {
    TRACE("Desk::AddModule()","start");
    GuiModule *guiModule = guiModuleFactory.CreateGuiModule( algorithm->GetModule(moduleId));
    
    guiModules.push_back(guiModule);
    gtkModules.push_back(NULL);
    entryModules.push_back(NULL);
    
    TRACE("Desk::AddModule()","end");

}


Module* Desk::FindModule(string name) {
    if (algorithm->GetModule(name) != NULL)
        return algorithm->GetModule(name);
    return NULL;
}

int Desk::FindOutput(string nameModule,string outName) {
    return FindOutput(FindModule(nameModule),outName);
}


int Desk::FindOutput(Module* module,string outName) {
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

int Desk::FindInput(string nameModule,string inName) {
    return FindInput(FindModule(nameModule),inName);
}

int Desk::FindInput(Module* module,string inName) {
    if (module == NULL ) return -1;
    for (int i = 0; i < module->GetInputCount(); i++)
        if (module->GetInput(i)->GetName() == inName)
            return module->GetInput(i)->GetID();
    return -1;
}

void Desk::SetPosition(string nameModule,int x, int y) {
    TRACE("Desk::SetPosition()","start");
    for (int i = 0;i<guiModules.size();i++)
        if (guiModules[i]->GetModule() != NULL)
        {
            TRACE("Desk::SetPosition()","znaleziono");
            if (guiModules[i]->GetModule()->GetName() == nameModule)
            {
                guiModules[i]->SetXY(x,y);                
            }
        }
    TRACE("Desk::SetPosition()","end");
}


GuiModule* Desk::GetDeskModuleActive() {
    return deskModuleActive;    
}

void Desk::SetDeskModuleActive(GuiModule* deskModule)
{
    
    if (deskModuleActive != NULL)
        for (int i = 0;i<guiModules.size();i++)
            if (guiModules[i] == deskModuleActive)
               gtkModules[i]->SetSelected(false);    
    deskModuleActive = deskModule;        
    
    for (int i = 0;i<guiModules.size();i++)
        if (guiModules[i] == deskModuleActive) {
            gtkModules[i]->SetSelected(true);  
            gtkModules[i]->on_expose_event(NULL);
        } 
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

void Desk::Clear() {
    TRACE("Desk::Clear()","start");
    for (int i = 0;i<gtkModules.size();i++)
    {
         delete entryModules[i];
         delete gtkModules[i];         
    }   
    TRACE("Desk::Clear()","1");
    gtkModules.clear(); 
    guiModules.clear(); 
    entryModules.clear(); 
    TRACE("Desk::Clear()","2");
    deskModuleActive = NULL;
    
    algorithm->Clear();
    TRACE("Desk::Clear()","3");
    
    AddAllModuleToDesk();
    TRACE("Desk::Clear()","end");

}


void Desk::DeleteActiveModule() { 
    TRACE("Desk::DeleteActiveModule()", "start");   
    algorithm->DeleteModule(algorithm->GetModuleId(GetDeskModuleActive()->GetModule()->GetName()));
    TRACE("Desk::DeleteActiveModule()", "1");
    
    vector<GuiModule*>::iterator iter1;
    vector<GtkModule*>::iterator iter2;
    vector<Gtk::Entry*>::iterator iter3;
    
    Gtk::Entry* entry;
    GtkModule*  widget;
    
    for (int i = 0;i<guiModules.size();i++)
        if (guiModules[i] == deskModuleActive) {
            entry  = entryModules[i];  
            widget = gtkModules[i];
        } 
    

    
    for (iter1 = guiModules.begin(); (*iter1)!=deskModuleActive; iter1++) {  
    }
    for (iter2 = gtkModules.begin(); (*iter2)!=widget; iter2++) {  
    }
    for (iter3 = entryModules.begin(); (*iter3)!=entry; iter3++) {  
    }

    
    if ((*iter1)==deskModuleActive) {
        TRACE("Desk::DeleteActiveModule()", "1.5");
              
        guiModules.erase(iter1);
        gtkModules.erase(iter2);
        entryModules.erase(iter3);
        TRACE("Desk::DeleteActiveModule()", "1.6");
        
        TRACE("Desk::DeleteActiveModule()", "2");
        delete deskModuleActive;
        delete entry;
        delete widget;
        deskModuleActive = NULL;

    }
    TRACE("Desk::DeleteActiveModule()", "end"); 
}

//Zapisywanie do plik ----------------------------------------------------------
void Desk::SaveToFile(string filename)
{
    //Zmienne do parsowania XML
	string xml =
	   "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
       "<algorithm name=\""+algorithm->GetName()+"\">\n";

    //czy sa jakies moduly ??
    if (guiModules.size()>0)
    {
        xml+= "  <modules>\n";
        for (int i = 0;i<guiModules.size();i++)
        {
            if ((guiModules[i]->GetModule()->GetType()!="audioportin")&(guiModules[i]->GetModule()->GetType()!="audioportout"))
            {
                xml+= "    <module type=\""+ guiModules[i]->GetModule()->GetType()+"\" name=\""+guiModules[i]->GetModule()->GetName()+"\">\n";            
                if (guiModules[i]->GetModule()->GetParameterCount()>0)
                    for (int p = 0;p<guiModules[i]->GetModule()->GetParameterCount();p++) {
                        
                        if (((Parameter*) guiModules[i]->GetModule()->GetParameter(p))->GetType() =="float") {                        
                            ParameterFloat* param =	(ParameterFloat*)guiModules[i]->GetModule()->GetParameter(p);
                            xml+= "        <parameter number=\""+IntToString(p)+"\" type=\"float\">"+IntToString(param->GetValue())+"</parameter>";
                        } else {
                            ParameterString* param = (ParameterString*)guiModules[i]->GetModule()->GetParameter(p);					
					       xml+= "        <parameter number=\""+IntToString(p)+"\" type=\"string\">"+param->GetText()+"</parameter>";
                        }               
                        
                    }            
                xml+= "    </module>";
            }
        }
        xml+= "  </modules>\n";
    }
    
    
    int input_, output_,c1_,c2_;
    xml+= "  <connections>\n";
    for (int c1=0; c1 < guiModules.size(); c1++) {
        for (int input = 0; input < guiModules[c1]->GetModule()->GetInputCount(); input++) {

            float *signal = guiModules[c1]->GetModule()->GetInput(input)->GetSignal();    
            
            for (int c2=0; c2 < guiModules.size(); c2++) {
                for (int output = 0; output < guiModules[c2]->GetModule()->GetOutputCount(); output++) {              
                    if (guiModules[c2]->GetModule()->GetOutput(output)->GetSignal() == signal) {
                        input_  = input;
                        output_ = output;
                        c1_ = c1;
                        c2_ = c2;
                    }
                }
            }
            xml+= "    <connection name1=\"" + guiModules[c2_]->GetModule()->GetName() +
                               "\" output=\"" + IntToString(output_) +
                               "\" name2=\"" + guiModules[c1_]->GetModule()->GetName() +
                               "\" input=\"" + IntToString(input_) + "\"/> \n";

        
        }    
    }
    
    xml+= "  </connections>\n";
    
    //czy sa jakies moduly ??
    if (guiModules.size()>0)
    {
        xml+= "  <modules_widget>\n";
        for (int i = 0;i<guiModules.size();i++)
            xml+= "    <module_widget name=\"" + guiModules[i]->GetModule()->GetName() +
                                      "\" x=\"" + IntToString(guiModules[i]->GetX()) +
                                      "\" y=\"" + IntToString(guiModules[i]->GetY()) + "\" />\n";
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
 

    for (int i = 0;i<gtkModules.size();i++)
    {
         delete gtkModules[i];
         delete entryModules[i];         
    }   
    
    guiModules.clear();
    gtkModules.clear();
    entryModules.clear(); 
    
    deskModuleActive = NULL;
    

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

