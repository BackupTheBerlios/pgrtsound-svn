#include "Scheduler.h"


using namespace std;

Scheduler::Scheduler(){
    isDebugMode = false;
}

Scheduler::~Scheduler(){	
}


CConnector *Scheduler::FindConnector(int id){
    for (IterListConn i = connectorList.begin(); i != connectorList.end(); i++ ) {    
        if ((*i)->id == id)
            return (*i);
       
    }
    cout << "Connector not found "<<endl;
    return NULL;
}
 
 
   
void Scheduler:: AddConnector(CConnector *conn){
  if (isDebugMode) 
    cout << "Add connector ";
    
   connectorList.push_back(conn);
  
  if (isDebugMode) 
    cout << "[OK]"<<endl;
}

void Scheduler::AddModule(Module *module){
  if (isDebugMode) 
    cout << "Add module ";
  
  int inputCount=0;
  for (int i = 0; i < module->inputCount; i++) {
    if (module->inConnection[i] != NULL) 
        inputCount++;    
  }
  
  //zapelnianie poloczen wejsciowych
  //wow!!! chyba troche dluga linia :)
  for (module->iterInConnection=module->inConnection.begin();module->iterInConnection!=module->inConnection.end();module->iterInConnection++) {
     (*(*(module->iterInConnection))->In())=1;  //a to juz chyba porazka ;)
  }
  
  moduleList.push_back(module);

  if (isDebugMode) 
    cout << module->name << " inputCount="<<inputCount<<"  maxInputCount="<<module->inputCount<< endl;
}

/****************************  AddTask **********************************/
void Scheduler::AddTask(int priority, Module *module){
	taskList.push(Task(priority, module));
   if (isDebugMode) 
        cout << "Add module " << module->name << "  priority = " << priority << endl;
}

/****************************  CreateTaskList **********************************/
void Scheduler::CreateTaskList(){
    bool stop = false; //zabezpieczenie przed zapetleniem    
    int priority = 0;  //priorytet danego bloku
    int inputCount;    //obliczona ilosc wejsc
    
    while (!stop) {
        stop = true;
        for (IterList i = moduleList.begin(); i != moduleList.end(); i++ ) {
            if (isDebugMode) 
                cout << "Priority " << priority << endl;
            if ((*i)!=NULL) {
                //zliczanie zerowych wejsc (czyli takich ktore sa NIE podlaczone)
                inputCount = 0;
                for ((*i)->iterInConnection=(*i)->inConnection.begin();(*i)->iterInConnection!=(*i)->inConnection.end();(*i)->iterInConnection++) {
                    if ((*(*((*i)->iterInConnection))->In())!=0) 
                        inputCount++;
                }
                if (inputCount==0) {
                    //zerowanie wszystkich wyjsc
                    for ((*i)->iterOutConnection=(*i)->outConnection.begin();(*i)->iterOutConnection!=(*i)->outConnection.end();(*i)->iterOutConnection++) {
                        (*(*((*i)->iterOutConnection))->In())=0;
                    }
                    stop = false;
                    cout << "Input "<<(*i)->name<< ": " <<(*i)->inputCount<<" " <<((*i)->inputCount)<<endl; 
                    AddTask(priority,(*i));
                    priority++;
                    (*i) = NULL;
                }
            }//endIf
        }//endFor
    }//endWhile
}


void Scheduler::Start(){
    vector<Task> v = taskList.getVector();
    copy(v.begin(), v.end(), ostream_iterator<Task>(cout, " "));
    cout << "\n-----------" << endl;

    time_t timeStart, timeStop;
    timeStart = time(0);
    for (int i = 0; i != 16*16*16 ; i++) {
      for (vector  <Task> :: iterator it = v.begin(); it != v.end(); it++) {
        (*it).module->Process();  
        //cout <<(*it).module->name<<endl;
      }
    //cout << "Counter = " << i << endl;
    }
    timeStop = time(0);
    float timeAver = (timeStop - timeStart);
    timeAver /=16*16*16;

    cout << "Time (start)   = " << timeStart << endl;
    cout << "Time (stop)    = " << timeStop << endl;
    cout << "Time           = " << timeStop - timeStart << "[sec]" <<endl;
    cout << "Time (average) = " << timeAver << "[sec]" << endl;    
        
}
