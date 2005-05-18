#include "sum.h"

Sum::Sum() {
	type = "sumator";

	i1.SetName("input 1");
	AddInput(&i1);
	
	i2.SetName("input 2");
	AddInput(&i2);
	
	oSum.SetName("sum");
	AddOutput(&oSum);
}

Sum::~Sum() {
	// insert your code here
}

void Sum::Process() {
	int n;
	float* in1 = i1.GetSignal();
	float* in2 = i2.GetSignal();
	float* sum = oSum.GetSignal();
	
	for(n = 0; n < Module::framesPerBlock; n++)	{
		*sum++ = (*in1++) + (*in2++);
	}	
}

