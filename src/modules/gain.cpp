#include "gain.h"

Gain::Gain() {
	type = "gain";
	iIn = addInput("input");
	oOut = addOutput("output");
	pGain = addParam("gain");
	setParam(pGain, 1);
}

Gain::~Gain() {
}

void Gain::process() {
	int n;
	float* in = inputs[iIn]->signal;
	float* out = outputs[oOut]->signal;
	float gain = params[pGain]->value;

	for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = (*in++) * gain;
	}
		
}
