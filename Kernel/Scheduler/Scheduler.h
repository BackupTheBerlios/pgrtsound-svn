#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <ctime>
#include <queue>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>

#include "../../Blocks/CBlock.h"
#include "Task.h"

using namespace std;

typedef list <CBlock *> List;
typedef List::iterator IterList;


template<class T> class PQV : public priority_queue<T> {
public:
  typedef vector<T> TVec;
  TVec getVector() {
    TVec r(this->c.begin(),this->c.end());
    sort_heap(r.begin(), r.end(), this->comp);
    reverse(r.begin(), r.end());
    return r;
  }
};

class Scheduler{
private:
  //parametry
    //lista zadañ w kolejnoœci
	PQV <Task> taskList;	 
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
    //Rozpoczêcie przetwarzania
    void Start();
	//konstruktor i destruktor
	Scheduler();
	virtual ~Scheduler();

	
};

#endif // SCHEDULER_H
