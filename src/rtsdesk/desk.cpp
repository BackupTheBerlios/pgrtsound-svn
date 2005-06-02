#include "desk.h"
#include "core_mp.h"
#include "xmlconfigfile.h"
#include "debug.h"

//Konstruktor i destruktor -----------------------------------------------------
Desk::Desk(Algorithm *algo)
{
    algorithm           = algo;
    deskModuleActive    = NULL;
}

Desk::~Desk()
{
    delete algorithm;
}

//Obs³uga modu³ów --------------------------------------------------------------
void Desk::AddModule(string type, string name)
{
    DeskModule *deskModule = new DeskModule(0,0);
    int id = algorithm->AddModule(type);
    algorithm->GetModule(id)->SetName(name);          
    deskModule->rtsModule  = algorithm->GetModule(id);
    deskModules.push_back(deskModule);
}

void Desk::AddModule(int id)
{
    DeskModule *deskModule = new DeskModule(0,0);          
    deskModule->rtsModule  = algorithm->GetModule(id);
    deskModules.push_back(deskModule);
}

int Desk::FindModule(string name)
{
    if (algorithm->GetModule(name)!=NULL)
        return algorithm->GetModule(name)->GetID();
    return -1;
}

string  Desk::FindModule(int id)
{
    for (int i = 0;i<deskModules.size();i++)
        if (deskModules[i]->rtsModule->GetID() == id)
            return (deskModules[i]->rtsModule->GetName());
    return "";
}

int Desk::FindOutput(string nameModule,string outName)
{
    return FindOutput(FindModule(nameModule),outName);
}


int Desk::FindOutput(int idModule,string outName)
{
    for (int i = 0; i < algorithm->GetModule(idModule)->GetOutputCount(); i++)
        if (algorithm->GetModule(idModule)->GetOutput(i)->GetName() == outName)
            return algorithm->GetModule(idModule)->GetOutput(i)->GetID();
    return -1;
}

int Desk::FindInput(string nameModule,string inName)
{
    return FindInput(FindModule(nameModule),inName);
}

int Desk::FindInput(int idModule,string inName)
{
    for (int i = 0; i < algorithm->GetModule(idModule)->GetInputCount(); i++)
        if (algorithm->GetModule(idModule)->GetInput(i)->GetName() == inName)
            return algorithm->GetModule(idModule)->GetInput(i)->GetID();
    return -1;
}

void Desk::SetPosition(string nameModule,int x, int y)
{
    for (int i = 0;i<deskModules.size();i++)
        if (deskModules[i]->rtsModule->GetName() == nameModule)
        {
            deskModules[i]->x=x;
            deskModules[i]->y=y;
        }

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
            xml+= "    <module type=\""+ deskModules[i]->rtsModule->GetType()+"\" name=\""+deskModules[i]->rtsModule->GetName()+"\">\n";            
            if (deskModules[i]->rtsModule->GetParameterCount()>0)
                for (int p = 0;p<deskModules[i]->rtsModule->GetParameterCount();p++)
                {
                    if ((deskModules[i]->rtsModule->GetParameter(p)->GetGUIType() == gtSlider) or 
                        (deskModules[i]->rtsModule->GetParameter(p)->GetGUIType() == gtProperty))
                    {
                        ParameterFloat* param =	(ParameterFloat*)deskModules[i]->rtsModule->GetParameter(p);
					    xml+= "        <parameter number=\""+IntToString(p)+"\" type=\"float\">"+IntToString(param->GetValue())+"</parameter>";
                    }
                
                    if (deskModules[i]->rtsModule->GetParameter(p)->GetGUIType() == gtEditBox)
                    {
                        ParameterString* param = (ParameterString*)deskModules[i]->rtsModule->GetParameter(p);					
					    xml+= "        <parameter number=\""+IntToString(p)+"\" type=\"string\">"+param->GetText()+"</parameter>";
                     }               
                }
            
            xml+= "    </module>";
        }
        xml+= "  </modules>\n";
    }
    
    xml+= "  <connections>\n";
    for (int c1=0; c1 < deskModules.size(); c1++)
    {
        for (int input = 0; input < deskModules[c1]->rtsModule->GetInputCount(); input++)
        {          
            for (int c2=0; c2 < deskModules.size(); c2++)
            {
                if (deskModules[c2]->rtsModule->GetID() == deskModules[c1]->rtsModule->GetInput(input)->GetIDModule())
                {
                xml+= "    <connection name1=\"" + deskModules[c2]->rtsModule->GetName() +
                              "\" output=\"" + IntToString(deskModules[c1]->rtsModule->GetInput(input)->GetIDModuleOutput()) +
                               "\" name2=\"" + deskModules[c1]->rtsModule->GetName() +
                               "\" input=\"" + IntToString(input) + "\"/> \n";
                }    
            }
        
        }    
    }
    xml+= "  </connections>\n";

    //czy sa jakies moduly ??
    if (deskModules.size()>0)
    {
        xml+= "  <modules_widget>\n";
        for (int i = 0;i<deskModules.size();i++)
            xml+= "    <modules_widget name=\"" + deskModules[i]->rtsModule->GetName() +
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

//Odczyt z pliku ---------------------------------------------------------------
void Desk::LoadFromFile(string filename)
{
    XMLConfigFile xmlConfig;
    
 	try {
        xmlConfig.OpenFile(filename.c_str());
        algorithm->Clear(); // test
		xmlConfig.LoadAlgorithm(algorithm);
    } catch (RTSError& error) {
        cout << "Error: " << error.what() << endl;
        exit(1);
    }
    
    for (int m=0; m < algorithm->GetModulesCount(); m++)
    {      
        AddModule(algorithm->GetModule(m)->GetID());
    }
     
    TiXmlNode* node                     = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* moduleElements        = 0;
    TiXmlElement* moduleXML             = 0;
    TiXmlElement* connectionElements    = 0;

    TiXmlDocument doc(filename.c_str());
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
      //  cout << "Error: " << endl;
     //   exit(1);
    }

    node = doc.FirstChild( "algorithm" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );

//moduly pozycje
    node = projectElement->FirstChildElement("modules_widget");
    assert( node );
    moduleElements = node->ToElement();
    assert( moduleElements  );

    for( moduleXML = moduleElements->FirstChildElement();
         moduleXML;
         moduleXML = moduleXML->NextSiblingElement() )
    {
        SetPosition(moduleXML->Attribute("name"),atoi(moduleXML->Attribute("x")),atoi(moduleXML->Attribute("y")));
	}

}
