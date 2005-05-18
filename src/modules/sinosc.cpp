#include "sinosc.h"

SinOsc::SinOsc() {
	iFreq = AddInput("frequency");
	//iAmp = addInput("amplitude");
	oOut = AddOutput("output signal");
	
	type = "sinosc";
	phase = 0;
	phaseIncrement = 0;
	n = 0;
}

SinOsc::~SinOsc() {
}

void SinOsc::Process() {
	float* freq = inputs[iFreq]->signal;
	//float* amp = getInput(iAmp);
	float* out = outputs[oOut]->signal;
	float sr = (float)Module::sampleRate;
	
	for(int i = 0; i < Module::framesPerBlock; i++) {
        n++;
		phaseIncrement = 2.0*M_PI*(*freq++/sr);
		*out++ = 0.5*sin(phase);
		phase += phaseIncrement;
	}
}
