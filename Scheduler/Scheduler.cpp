#include "Scheduler.h"


using namespace std;

Scheduler::Scheduler(){
    isDebugMode = false;
}

Scheduler::~Scheduler(){	
}

void Scheduler::AddBlock(CBlock *block){
  if (isDebugMode) 
    cout << "Add block " << block->name << "  maxInputCount="<<block->inputCount<< endl;
  
  int inputCount=0;
  for (int i = 0; i < block->inputCount; i++) {
    if (block->inConnection[i] != NULL) 
        inputCount++;    
  }
  
  BlockInfo *blockInfo  = new BlockInfo();  
  blockInfo->block      = block;
  blockInfo->inputCount = inputCount;
  blockList.push_back(blockInfo);
  if (isDebugMode) 
    cout << "Add block " << block->name << " inputCount="<<inputCount<<"  maxInputCount="<<block->inputCount<< endl;
}

/****************************  AddTask **********************************/
void Scheduler::AddTask(int priority, CBlock *block){
	taskList.push(Task(priority, block));
   if (isDebugMode) 
        cout << "Add block " << block->name << "  priority = " << priority << endl;
}

/****************************  CreateTaskList **********************************/
void Scheduler::CreateTaskList(){
    int priority = 0;
    
	for (Iter i = blockList.begin(); i != blockList.end(); i++ ) {
       // if ( ((*i)!=NULL) & (*((*i)->inputCount) == 0) ) {
            cout << "Input "<<(*i)->block->name<< ": " <<(*i)->block->inputCount<<" " <<((*i)->inputCount)<<endl; 
       //     AddTask(priority,(*i));
       //     priority++;
       //     (*i) = NULL;
       // }
       
    
    } 
}
