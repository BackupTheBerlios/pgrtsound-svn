#ifndef LOADER_H
#define LOADER_H

//XML
#include "../../../library/TinyXML/tinyxml.h"

#ifdef TIXML_USE_STL
  #include <iostream>
   #include <sstream>
    using namespace std;
#else
   #include <stdio.h>
#endif 

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
    void LoadFromFile(string filename);
	
	//Konstruktor i destruktor
	Loader();
	virtual ~Loader();
};

#endif // LOADER_H
