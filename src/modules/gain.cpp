#include "gain.h"

Gain::Gain() :
	Module("New gain"),
	iIn("input"),
	oOut("output"),
	pGain("Wzmocnienie")
 {
	AddInput(iIn);
	AddOutput(oOut);
	pGain.Bound(0, 1, 0.01);    // ograczniczenie wartosci
	AddParameter(pGain);
}

Gain::~Gain() {
}

void Gain::Process() {
	unsigned long n;
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();
	float gain = pGain.GetValue();

	for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = (*in++) * gain;
	}
}
