#include "gain.h" // class's header file

// class constructor
Gain::Gain() {
	type = "gain";
	iIn = addInput("input");
	oOut = addOutput("output");
	pGain = addParam("gain");
	setParam(pGain, 1);
}

// class destructor
Gain::~Gain() {
}

void Gain::process() {
	int n;
	float* in = input(iIn).signal;
	float* out = output(oOut).signal;
	float gain = param(pGain);

	for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = (*in++) * gain;
	}
		
}
