#include "Adder.h"

Adder::Adder() {
    inputCount  = 2;
    outputCount = 1; 
    paramCount  = 0;  
}
Adder::~Adder()
{}
// override Process()
void Adder::Process(void)
{
   float *inBuff0 = inConnection[0]->Out();
   float *inBuff1 = inConnection[1]->Out();
   
   float *outBuff = outConnection[0]->In();
 
    for (int i=0;i<BUFFOR_SIZE;i++)
    {        
        *outBuff++ = (*inBuff0++) + (*inBuff1++) ;
    }    
}
