#ifndef LOADER_H
#define LOADER_H

//Bloki
#include "../Blocks/CBlock.h"
#include "../Blocks/CConnector.h"
#include "../Blocks/Adder.h"
#include "../Blocks/Amplifier.h"
#include "../Blocks/Generator.h"


//Scheduler
#include "..\Scheduler\Scheduler.h"


class Loader{
public:
//Atrybuty
	
	
//Metody
	//demo -- czyli przykladowy system	
	void Demo(void);
	
	//Konstruktor i destruktor
	Loader();
	virtual ~Loader();
};

#endif // LOADER_H
