#include "sinosc.h"

SinOsc::SinOsc() {
	iFreq = addInput("frequency");
	//iAmp = addInput("amplitude");
	oOut = addOutput("output signal");
	
	type = "sinosc";
	phase = 0;
	phaseIncrement = 0;
	n = 0;
}

SinOsc::~SinOsc() {
}

void SinOsc::process() {
	
	float* freq = input(iFreq).signal;
	//float* amp = getInput(iAmp);
	float* out = output(oOut).signal;
	
	for(int i = 0; i < Module::framesPerBlock; i++) {
        n++;
		phaseIncrement = 2.0*M_PI*(*freq++/(float)Module::sampleRate);
		*out++ = 0.5*sin(phase);
		phase += phaseIncrement;
	}
}
