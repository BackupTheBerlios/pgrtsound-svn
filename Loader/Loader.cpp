#include "Loader.h"

Loader::Loader()
{}
Loader::~Loader()
{}


void Loader::Demo(void)
{
/*Wygl¹d
 * 
 *     GEN --> Amp 
 * 
 */
 
int FRAMES_PER_BUFFER=256;

//Scheduler

    cout << "Demo started..." << endl;

    Scheduler *projekt = new Scheduler();
    projekt -> isDebugMode = true;

//utworzenie przewodow
    CConnector *conn1 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn2 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn3 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn4 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn5 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn6 = new CConnector(FRAMES_PER_BUFFER);    
    CConnector *conn7 = new CConnector(FRAMES_PER_BUFFER);    

    //inicjowanie generatorów
    Generator *gen          = new Generator();
    gen->name               = "Gen1";
    gen->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    gen->param              = new float[gen->paramCount];
    gen->param[0]           = 0.25;
    gen->param[1]           = 440./44100.;
    gen->param[2]           = 0;
    gen->outConnection.push_back(conn1);
    projekt->AddBlock(gen);
    
    gen          = new Generator();
    gen->name               = "Gen2";
    gen->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    gen->param              = new float[gen->paramCount];
    gen->param[0]           = 0.25;
    gen->param[1]           = 440./44100.;
    gen->param[2]           = 0;
    gen->outConnection.push_back(conn3);
    projekt->AddBlock(gen);
    
    gen          = new Generator();
    gen->name               = "Gen3";
    gen->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    gen->param              = new float[gen->paramCount];
    gen->param[0]           = 0.25;
    gen->param[1]           = 440./44100.;
    gen->param[2]           = 0;
    gen->outConnection.push_back(conn5);
    projekt->AddBlock(gen);
    
     //inicjowanie wzmacniaczy
    Amplifier *amp          = new Amplifier();
    amp->inputCount         = 1;
    amp->name               = "Amp1";
    amp->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    amp->param              = new float[amp->paramCount];
    amp->param[0]           = 2.;     
    amp->inConnection.push_back(conn1);
    amp->outConnection.push_back(conn2);
    projekt->AddBlock(amp);

    amp          = new Amplifier();
    amp->inputCount         = 1;
    amp->name               = "Amp2";
    amp->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    amp->param              = new float[amp->paramCount];
    amp->param[0]           = 2.;     
    amp->inConnection.push_back(conn5);
    amp->outConnection.push_back(conn6);
    projekt->AddBlock(amp);        

    //inicjowanie sumatorow
    Adder *add              = new Adder();
    add->name               = "Add1";
    add->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    add->inConnection.push_back(conn2);
    add->inConnection.push_back(conn3);
    add->outConnection.push_back(conn4); 
    projekt->AddBlock(add);
    
    add              = new Adder();
    add->name               = "Add2";
    add->BUFFOR_SIZE        = FRAMES_PER_BUFFER;
    add->inConnection.push_back(conn4);
    add->inConnection.push_back(conn6);
    add->outConnection.push_back(conn7); 
    projekt->AddBlock(add);
    
    
    projekt->CreateTaskList();
    

}
