#include "multiplication.h"

Multiplication::Multiplication() : Module("multiplication", "New multiplication"),
	iIn1("input 1"), iIn2("input 2"), oOut("out")
{
   	AddInput(&iIn1);
	AddInput(&iIn2);
	AddOutput(&oOut);
}

// class destructor
Multiplication::~Multiplication(){
}

void Multiplication::Process() {
	int n;
	float* in1 = iIn1.GetSignal();
	float* in2 = iIn2.GetSignal();
	float* out = oOut.GetSignal();

	for(n = 0; n < Module::framesPerBlock; n++)	{
		*out++ = (*in1++) * (*in2++);
	}
}
