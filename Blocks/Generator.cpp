
#include "Generator.h"  // class's header file
     //zmienne systemowe

#include <math.h>
#include <iostream>

using namespace std;
// class constructor
Generator::Generator()
{
    n = 1;	
    inputCount   = new int(1);
    paramCount   = new int(3); 
}

// class destructor
Generator::~Generator()
{

}

// override Process()
void Generator::Process(void)
{
	float *outBuff = outConnection->In();
	
    for (int i = 0; i < BUFFOR_SIZE; i++)
    {        
        *outBuff++ = param[0]*sin(6.2831853*param[1]*(float)n);
        n++;
    }    
}


