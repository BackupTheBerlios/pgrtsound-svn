#include "Loader.h"
Loader::Loader(){
    FRAMES_PER_BUFFER = 256;
}
Loader::~Loader()
{}


void Loader::LoadFromFile(string filename)
{
   //wsk. do objektow tworzonych
    CConnector *conn    = NULL;
    CBlock *block       = NULL;
    
    
   //XML
    TiXmlNode* node                     = 0;
    TiXmlElement* projectElement        = 0;
    TiXmlElement* connectionElements    = 0;
    TiXmlElement* blockElements         = 0;
    TiXmlElement* element               = 0;
    TiXmlElement* blockXML              = 0;
    
    
    string blockType;
    string blockName;
    string strTemp;
    int blockId;
    
    
    TiXmlDocument doc(filename);
    bool loadOkay = doc.LoadFile();

    if ( !loadOkay )
    {
        cout << "Could not load test file '" << filename << "'. Error='%s'. Exiting.\n" << doc.ErrorDesc()<<endl;
        exit( 1 );
    } 
    
    node = doc.FirstChild( "project" );
    assert( node );
    projectElement = node->ToElement();
    assert( projectElement );
    
    //*******************************
    //Stworzenie obiekt�w connections
    //*******************************
    node = projectElement->FirstChildElement("connections");
    assert( node );
    connectionElements = node->ToElement();
    assert( connectionElements );
   
    for( element = connectionElements->FirstChildElement();
         element;
         element = element->NextSiblingElement() )
    {
        cout << element ->Attribute("name");
        
        conn = new CConnector(FRAMES_PER_BUFFER);
        conn->name = element ->Attribute("name");
        conn->id  = atoi(element ->Attribute("id"));
        
        project->AddConnector(conn);
        cout << "      [Created]" <<endl;
        
    }
    
    
    //*******************************    
    //Stworzenie obiekt�w blokow
    //*******************************
    node = projectElement->FirstChildElement("blocks");
    assert( node );
    blockElements = node->ToElement();
    assert( blockElements  );
    string idStr;
    for( blockXML = blockElements->FirstChildElement();
         blockXML;
         blockXML = blockXML->NextSiblingElement() )
    {
        cout << blockXML->Attribute("type");
        
        blockType = blockXML->Attribute("type");
        blockName = blockXML->Attribute("name");
        blockId   = atoi(blockXML->Attribute("id"));

        block = NULL;
        if (blockType=="Generator") {
            block = new Generator();    
        }
        if (blockType=="Amplifier") {
            block = new Amplifier();    
        }
        if (blockType=="Adder") {
            block = new Adder();    
        }
        if (blockType=="FixedParameter") {
            block = new FixedParameter();    
        }
        
                
        if (block != NULL) {
            block -> inConnection.reserve(block->inputCount);
            block -> outConnection.reserve(block->outputCount);
            block -> name = blockName;
            block -> id   = blockId;
            block -> type = blockType;
            if (block->paramCount>0)
                block->param  = new float[block->paramCount];
             
            for( element = blockXML->FirstChildElement();
                 element;
                 element = element->NextSiblingElement() ) {
    
                strTemp = element->Value();
                cout <<"-"<<strTemp<<"-"<<endl;
                if (strTemp=="input") {
                    cout <<atoi(element -> Attribute("number"))-1<<endl;
                    block -> inConnection[atoi(element -> Attribute("number"))-1] = NULL;//project->FindConnector(atoi(element -> Attribute("idConnection")));                    
                }
                if (strTemp=="output") {
                    cout <<"*"<<endl;
                    block ->   outConnection[atoi(element -> Attribute("number"))] = project->FindConnector(atoi(element -> Attribute("idConnection")));
                }
                if (strTemp=="parameter") {
                    cout <<"*"<<endl;                   
                    block ->   param[atoi(element -> Attribute("number"))] = atof(element -> Attribute("value"));
                }
            
            }
        }
         
        
        if (block != NULL){
           project->AddBlock(block);
            cout << "      [Created]" <<endl;
        }
        
    }
    
}


void Loader::Demo(void)
{
/*Wygl�d
 * 
 *     GEN --> Amp 
 * 
 */
 
int FRAMES_PER_BUFFER=256;

//Scheduler

    cout << "[DEMO]" << endl;
     

//utworzenie przewodow
    CConnector *conn01 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn02 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn03 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn04 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn05 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn06 = new CConnector(FRAMES_PER_BUFFER);    
    CConnector *conn07 = new CConnector(FRAMES_PER_BUFFER); 
    CConnector *conn08 = new CConnector(FRAMES_PER_BUFFER);    
    CConnector *conn09 = new CConnector(FRAMES_PER_BUFFER);    
    CConnector *conn10 = new CConnector(FRAMES_PER_BUFFER);        
    CConnector *conn11 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn12 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn13 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn14 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn15 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn16 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn17 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn18 = new CConnector(FRAMES_PER_BUFFER);
    
    //inicjowanie generator�w
//    
    Generator *gen          = new Generator();
    gen->name               = "Gen1";
    gen->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //in
    gen->inConnection.push_back(conn08);
    gen->inConnection.push_back(conn09);
    gen->inConnection.push_back(conn10);
    //out
    gen->outConnection.push_back(conn01);    
    
    project->AddBlock(gen);
    
//  
    gen          = new Generator();
    gen->name               = "Gen2";
    gen->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //in
    gen->inConnection.push_back(conn12);
    gen->inConnection.push_back(conn13);
    gen->inConnection.push_back(conn14);
    //out
    gen->outConnection.push_back(conn03);
    
    project->AddBlock(gen);

//   
    gen          = new Generator();
    gen->name               = "Gen3";
    gen->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //in
    gen->inConnection.push_back(conn15);
    gen->inConnection.push_back(conn16);
    gen->inConnection.push_back(conn17);            
    //out
    gen->outConnection.push_back(conn05);
    
    project->AddBlock(gen);
  
  
    //inicjowanie wzmacniaczy  
//    
    Amplifier *amp          = new Amplifier();
    amp->inputCount         = 1;
    amp->name               = "Amp1";
    amp->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //in
    amp->inConnection.push_back(conn01);
    amp->inConnection.push_back(conn11);
    //out
    amp->outConnection.push_back(conn02);
    
    project->AddBlock(amp);
    
//
    amp          = new Amplifier();
    amp->inputCount         = 1;
    amp->name               = "Amp2";
    amp->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //in
    amp->inConnection.push_back(conn05);
    amp->inConnection.push_back(conn18);
    //out
    amp->outConnection.push_back(conn06);
    
    project->AddBlock(amp);        


    //inicjowanie sumatorow
//
    Adder *add              = new Adder();
    add->name               = "Add1";
    add->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //in
    add->inConnection.push_back(conn02);
    add->inConnection.push_back(conn03);
    //out
    add->outConnection.push_back(conn04); 
    
    project->AddBlock(add);
    
//    
    add                     = new Adder();
    add->name               = "Add2";
    add->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //in
    add->inConnection.push_back(conn04);
    add->inConnection.push_back(conn06);
    //out
    add->outConnection.push_back(conn07); 
    
    project->AddBlock(add);
    
    //FixedParameter
//
    FixedParameter *fix     = new FixedParameter();
    fix->name               = "Fix1";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 0.25;
    //in
    fix->outConnection.push_back(conn08);
    
    project->AddBlock(fix); 
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix2";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 440./44100.;
    //in
    fix->outConnection.push_back(conn09); 
    
    project->AddBlock(fix);
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix3";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 0.;
    //in
    fix->outConnection.push_back(conn10);
    
    project->AddBlock(fix);  
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix4";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 0.25;
    //in
    fix->outConnection.push_back(conn12);
    
    project->AddBlock(fix); 
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix5";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 440./44100.;
    //in
    fix->outConnection.push_back(conn13); 
    
    project->AddBlock(fix);
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix6";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 0.;
    //in
    fix->outConnection.push_back(conn14);
    
    project->AddBlock(fix);      
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix7";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 0.25;
    //in
    fix->outConnection.push_back(conn15);
    
    project->AddBlock(fix); 
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix8";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 440./44100.;
    //in
    fix->outConnection.push_back(conn16); 
    
    project->AddBlock(fix);
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix9";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 0.;
    //in
    fix->outConnection.push_back(conn17);
    
    project->AddBlock(fix);      
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix10";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 2.;
    //in
    fix->outConnection.push_back(conn11);
    
    project->AddBlock(fix);      
    
//
    fix                     = new FixedParameter();
    fix->name               = "Fix11";
    fix->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    //param
    fix->param              = new float[fix->paramCount];
    fix->param[0]           = 0.2;
    //in
    fix->outConnection.push_back(conn18);
    
    project->AddBlock(fix);         
     

}
