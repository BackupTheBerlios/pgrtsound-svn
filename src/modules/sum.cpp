#include "sum.h"

Sum::Sum() {
	type = "sumator";
	i1 = AddInput("input 1");
	i2 = AddInput("input 2");
	oSum = AddOutput("sum");
}

Sum::~Sum() {
	// insert your code here
}

void Sum::Process() {
	int n;
	float* in1 = inputs[i1]->signal;
	float* in2 = inputs[i2]->signal;
	float* sum = outputs[oSum]->signal;
	
	for(n = 0; n < Module::framesPerBlock; n++)
	{
		*sum++ = (*in1++) + (*in2++);
	}	
}

