#include "sum.h"

Sum::Sum() : Module("New sumator"),
	i1("input 1"), i2("input 2"), oSum("sum")
{
	AddInput(i1);
	AddInput(i2);
	AddOutput(oSum);
}

Sum::~Sum() {
}

string Sum::GetTypeStatic() {
	return "sumator";
}

string Sum::GetType() {
	return "sumator";
}

Module* Sum::Create() {
	return new Sum();
}

void Sum::Process() {
	float* in1 = i1.GetSignal();
	float* in2 = i2.GetSignal();
	float* sum = oSum.GetSignal();
	
	for(unsigned long n = 0; n < Module::framesPerBlock; n++)	{
		*sum++ = (*in1++) + (*in2++);
	}	
}

