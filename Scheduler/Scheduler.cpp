#include "Scheduler.h"


using namespace std;

Scheduler::Scheduler(){
    isDebugMode = false;
}

Scheduler::~Scheduler(){	
}

void Scheduler::AddBlock(CBlock *block){
  if (isDebugMode) 
    cout << "Add block ";
  
  int inputCount=0;
  for (int i = 0; i < block->inputCount; i++) {
    if (block->inConnection[i] != NULL) 
        inputCount++;    
  }
  
  //zapelnianie poloczen wejsciowych
  //wow!!! chyba troche dluga linia :)
  for (block->iterInConnection=block->inConnection.begin();block->iterInConnection!=block->inConnection.end();block->iterInConnection++) {
     (*(*(block->iterInConnection))->In())=1;  //a to juz chyba porazka ;)
  }
  
  blockList.push_back(block);

  if (isDebugMode) 
    cout << block->name << " inputCount="<<inputCount<<"  maxInputCount="<<block->inputCount<< endl;
}

/****************************  AddTask **********************************/
void Scheduler::AddTask(int priority, CBlock *block){
	taskList.push(Task(priority, block));
   if (isDebugMode) 
        cout << "Add block " << block->name << "  priority = " << priority << endl;
}

/****************************  CreateTaskList **********************************/
void Scheduler::CreateTaskList(){
    bool stop = false; //zabezpieczenie przed zapetleniem    
    int priority = 0;  //priorytet danego bloku
    int inputCount;    //obliczona ilosc wejsc
    
    while (!stop) {
        stop = true;
        for (IterList i = blockList.begin(); i != blockList.end(); i++ ) {
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
