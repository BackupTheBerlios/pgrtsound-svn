
#include "Generator.h"  // class's header file
     //zmienne systemowe

#include <math.h>
#include <iostream>

using namespace std;
// class constructor
Generator::Generator()
{
    n = 1;	
    inputCount   = 3;
    outputCount  = 1;
    paramCount  = 0;
}

// class destructor
Generator::~Generator()
{

}

// override Process()
void Generator::Process(void)
{
    float *inBuff0 = inConnection[0]->Out();
    float *inBuff1 = inConnection[1]->Out();
    float *inBuff2 = inConnection[2]->Out();
	float *outBuff = outConnection[0]->In();
	
    for (int i = 0; i < BUFFOR_SIZE; i++)
    {        
        *outBuff++ = (*inBuff0++)*sin(6.2831853*(*inBuff1++)*(float)n+(*inBuff2++));
        n++;
    }    
}


