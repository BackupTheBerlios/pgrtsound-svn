
#include "amplifier.h"  // class's header file

// class constructor
Amplifier::Amplifier()
{
    inputCount  = 1;
    outputCount = 1;
    paramCount  = 0;
}

// class destructor
Amplifier::~Amplifier()
{
	// insert your code here
}

// override Process()
void Amplifier::Process(void)
{
	float *inBuff0 = inConnection[0]->Out();
    float *inBuff1 = inConnection[1]->Out();
	float *outBuff = outConnection[0]->In();
	
    for (int i=0;i<BUFFOR_SIZE;i++)
    {        
        *outBuff++ = (*inBuff0++) * (*inBuff1++) ;
    }    
}
