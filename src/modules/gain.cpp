#include "gain.h"

Gain::Gain() {
	type = "gain";
	iIn = AddInput("input");
	oOut = AddOutput("output");
	pGain = AddParam("gain");
	SetParam(pGain, 1);
}

Gain::~Gain() {
}

void Gain::Process() {
	int n;
	float* in = inputs[iIn]->signal;
	float* out = outputs[oOut]->signal;
	float gain = params[pGain]->value;

	for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = (*in++) * gain;
	}
		
}
