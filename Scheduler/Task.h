#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>
#include "../Blocks/CBlock.h"

using namespace std;

class Task{
public:
  //parametry
	int priority;  //Piorytet podsatwowy
	CBlock *block;  //Adres do obiektu
	
  //metody
	//metoda sluzaca po porownywania priorytetow zadan (Task)
	friend bool operator < (const Task& task1, const Task& task2) {
		if (task1.priority>task2.priority) return true;
		return false;
	}
	// metoda do np w cout << task;
	friend ostream& operator << (ostream& os, const Task& task) {
		return os << task.priority  << ": " << task.block->name;
	}
		
	
	
	//konstruktor i destruktor
	Task(int priority1, CBlock *block1)
	: priority(priority1),block(block1){};
	virtual ~Task();
	
};

#endif // TASK_H
