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
    //lista zadañ w kolejnoœci
	priority_queue <Task> taskList;	 
	//lista bloków w sytemie
	List blockList;      
 
    

	
  //metdoy
	void AddTask(int priority, CBlock *block);
		
public:
  //parametry	
    bool isDebugMode;
  //metdoy
	//dodawanie bloku do listy blokow
	void AddBlock(CBlock *block);
	//tworzenie listy zadañ
	void CreateTaskList();
	//konstruktor i destruktor
	Scheduler();
	virtual ~Scheduler();

	
};

#endif // SCHEDULER_H
