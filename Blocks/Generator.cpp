
#include "Generator.h"  // class's header file
     //zmienne systemowe

#include <math.h>
#include <iostream>

using namespace std;
// class constructor
Generator::Generator()
{
    n = 1;	
    inputCount   = 0;
    outputCount  = 1;
    paramCount   = 3; 
}

// class destructor
Generator::~Generator()
{

}

// override Process()
void Generator::Process(void)
{
	float *outBuff = outConnection[0]->In();
	
    for (int i = 0; i < BUFFOR_SIZE; i++)
    {        
        *outBuff++ = param[0]*sin(6.2831853*param[1]*(float)n);
        n++;
    }    
}


