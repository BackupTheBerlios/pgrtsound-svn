#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <ctime>
#include <queue>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>

#include "../../Modules/Module.h"
#include "../../Modules/CConnector.h"
#include "Task.h"

using namespace std;

typedef list <Module *> List;
typedef List::iterator IterList;

typedef list <CConnector *> ListConn;
typedef ListConn::iterator IterListConn;

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
    //lista zada� w kolejno�ci
	PQV <Task> taskList;	 
	//lista blok�w w projekcie
	List moduleList;      
    //lista blok�w w projekcie
    ListConn connectorList;
    

	
  //metdoy
	void AddTask(int priority, Module *module);
		
public:
  //parametry	
    bool isDebugMode;
  //metdoy
	//dodawanie bloku do listy blokow
	void AddModule(Module *module);
    //dodawanie polaczenia do listy polaczen
    void AddConnector(CConnector *conn);
    //szukanie po��czenia od danym id
    CConnector *FindConnector(int id);
	//tworzenie listy zada�
	void CreateTaskList(); 
    //Rozpocz�cie przetwarzania
    void Start();
	//konstruktor i destruktor
	Scheduler();
	virtual ~Scheduler();

	
};

#endif // SCHEDULER_H
