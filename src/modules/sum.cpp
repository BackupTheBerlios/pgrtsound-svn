#include "sum.h"

// class constructor
Sum::Sum() {
	type = "sumator";
	i1 = addInput("input 1");
	i2 = addInput("input 2");
	oSum = addOutput("sum");
}

// class destructor
Sum::~Sum() {
	// insert your code here
}

void Sum::process() {
	int n;
	float* in1 = input(i1).signal;
	float* in2 = input(i2).signal;
	float* sum = output(oSum).signal;
	
	for(n = 0; n < Module::framesPerBlock; n++)
	{
		*sum++ = (*in1++) + (*in2++);
	}	
}

