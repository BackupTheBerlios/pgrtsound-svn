#ifndef LOADER_H
#define LOADER_H

//XML
#include "tinyxml.h"


  #include <iostream>
   #include <sstream>
    using namespace std;

#include <string>

//Bloki
#include "../../Modules/Module.h"
#include "../../Modules/CConnector.h"
#include "../../Modules/Adder.h"
#include "../../Modules/Amplifier.h"
#include "../../Modules/Generator.h"
#include "../../Modules/FixedParameter.h"


//Scheduler
#include "../Scheduler/Scheduler.h"


class Loader{
public:
//Atrybuty
	Scheduler *project;
    int FRAMES_PER_BUFFER;
	
//Metody
	//demo -- czyli przykladowy system	
	void Demo(void);
    //load form file
    void LoadFromFile(const char * filename);
	
	//Konstruktor i destruktor string
	Loader();
	virtual ~Loader();
};

#endif // LOADER_H
