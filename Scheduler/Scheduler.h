#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <queue>
#include <list>
#include <vector>
#include <string>

#include "../Blocks/CBlock.h"
#include "Task.h"

using namespace std;

struct BlockInfo {
    CBlock *block;
    int inputCount;   
};

typedef list <BlockInfo *> List;
typedef List::iterator Iter;

class Scheduler{
private:
  //parametry
    //lista zada� w kolejno�ci
	priority_queue <Task> taskList;	 
	//lista blok�w w sytemie
	List blockList;      
 
    

	
  //metdoy
	void AddTask(int priority, CBlock *block);
		
public:
  //parametry	
    bool isDebugMode;
  //metdoy
	//dodawanie bloku do listy blokow
	void AddBlock(CBlock *block);
	//tworzenie listy zada�
	void CreateTaskList();
	//konstruktor i destruktor
	Scheduler();
	virtual ~Scheduler();

	
};

#endif // SCHEDULER_H
