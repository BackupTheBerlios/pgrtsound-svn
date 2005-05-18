#include "gain.h"

Gain::Gain() {
	type = "gain";

	iIn.SetName("input");
	AddInput(&iIn);

	oOut.SetName("output");
	AddOutput(&oOut);

	pGain.SetName("gain");
	pGain.Bound(0, 1, 0.01);    // ograczniczenie wartosci
	pGain.SetGUIType(gtSlider);
	AddParameter(&pGain);
}

Gain::~Gain() {
}

void Gain::Process() {
	int n;
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();
	float gain = pGain.GetValue();

	for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = (*in++) * gain;
	}
		
}
