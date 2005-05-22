#include "slider.h"

// class constructor
Slider::Slider() : Module("slider", "New Slider"),
	oOut("wy"), pMin("min", gtProperty), pMax("max", gtProperty), pValue("value", gtSlider)
{
	AddOutput(&oOut);
	AddParameter(&pMin);
	AddParameter(&pMax);
	AddParameter(&pValue);
}


Slider::~Slider() {
}

void Slider::Init() {
   	pValue.Bound(pMin.GetValue(), pMax.GetValue(), 0.1);
}

void Slider::Process() {
	int n;
	float* out = oOut.GetSignal();
	//float delta = pMax.GetValue() - pMin.GetValue();
	//float value = pMin.GetValue() + ( delta * pValue.GetValue() );
	float value = pValue.GetValue();

	for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = value;
	}
}
