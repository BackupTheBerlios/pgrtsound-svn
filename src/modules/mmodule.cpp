#include "mmodule.h"

MModule::MModule() : Module("New module"),
	i1("input 1"), i2("input 2"), oModule("module")
{
	AddInput(i1);
	AddInput(i2);
	AddOutput(oModule);
}

MModule::~MModule() {
}

void MModule::Process() {
	float* in1 = i1.GetSignal();
	float* in2 = i2.GetSignal();
	float* module = oModule.GetSignal();
	
	for(unsigned long n = 0; n < Module::framesPerBlock; n++)	{
		*module++ = sqrt((*in1++)*(*in1++) + (*in2++)*(*in2++));
	}	
}

