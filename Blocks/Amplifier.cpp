
#include "amplifier.h"  // class's header file

// class constructor
Amplifier::Amplifier()
{
    inputCount  = new int(1);
    paramCount  = new int(1);
}

// class destructor
Amplifier::~Amplifier()
{
	// insert your code here
}

// override Process()
void Amplifier::Process(void)
{
	float *inBuff = inConnection->Out();
	float *outBuff = outConnection->In();
	
    for (int i=0;i<BUFFOR_SIZE;i++)
    {        
        *outBuff++ = (*inBuff++) * param[0] ;
    }    
}
