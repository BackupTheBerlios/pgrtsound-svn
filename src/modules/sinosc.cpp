#include "sinosc.h" // class's header file

// class constructor
SinOsc::SinOsc() {
	iFreq = addInput("frequency");
	//iAmp = addInput("amplitude");
	oOut = addOutput("output signal");
	
	type = "sinosc";
	phase = 0;
	phaseIncrement = 0;
}

// class destructor
SinOsc::~SinOsc() {
}

void SinOsc::process() {
	int n;
	float* freq = input(iFreq).signal;
	//float* amp = getInput(iAmp);
	float* out = output(oOut).signal;
	
	for(n = 0; n < Module::framesPerBlock; n++) {
		phaseIncrement = 2.0*M_PI*(*freq++/(float)Module::sampleRate);
		*out++ = 0.5*sin(phase);
		phase += phaseIncrement;
	}
}
