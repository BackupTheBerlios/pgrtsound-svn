#ifndef LOADER_H
#define LOADER_H

//XML
#include "../../TinyXML/tinyxml.h"

#ifdef TIXML_USE_STL
  #include <iostream>
   #include <sstream>
    using namespace std;
#else
   #include <stdio.h>
#endif 


//Bloki
#include "../../Blocks/CBlock.h"
#include "../../Blocks/CConnector.h"
#include "../../Blocks/Adder.h"
#include "../../Blocks/Amplifier.h"
#include "../../Blocks/Generator.h"
#include "../../Blocks/FixedParameter.h"


//Scheduler
#include "../Scheduler/Scheduler.h"


class Loader{
public:
//Atrybuty
	Scheduler *project;
	
//Metody
	//demo -- czyli przykladowy system	
	void Demo(void);
    //load form file
    void LoadFromFile (string filename);
	
	//Konstruktor i destruktor
	Loader();
	virtual ~Loader();
};

#endif // LOADER_H
