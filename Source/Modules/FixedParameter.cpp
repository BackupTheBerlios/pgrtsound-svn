#include "FixedParameter.h"

FixedParameter::FixedParameter()
{
    inputCount   = 0;
    outputCount  = 1;
    paramCount   = 1;
}

FixedParameter::~FixedParameter()
{}

void FixedParameter::Process(void)
{
    float *outBuff = outConnection[0]->In();
 
    for (int i=0;i<BUFFOR_SIZE;i++)
    {        
        *outBuff++ = param[0] ;
    }    
}
