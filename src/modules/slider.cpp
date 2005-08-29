#include "slider.h"

// class constructor
Slider::Slider() : Module("New Slider"),
	oOut("wy"), pMin("min"), pMax("max"), pValue("value")
{
	AddOutput(oOut);
	
	pMin.SetValue(0.0);
	pMax.SetValue(1.0);
	pValue.SetValue(0.0);

	// TODO: zaimplementowac 'step'
	pValue.Bound(pMin.GetValue(), pMax.GetValue(), 0.1f);

	AddParameter(pMin);
	AddParameter(pMax);
	AddParameter(pValue);

}

Slider::~Slider() {
}

string Slider::GetTypeStatic() {
	return "slider";
}

string Slider::GetType() {
	return "slider";
}

Module* Slider::Create() {
	return new Slider;
}

void Slider::Init() {
   	pValue.Bound(pMin.GetValue(), pMax.GetValue(), 0.1f);
}

void Slider::Process() {
	float* out = oOut.GetSignal();
	//float delta = pMax.GetValue() - pMin.GetValue();
	//float value = pMin.GetValue() + ( delta * pValue.GetValue() );
	float value = pValue.GetValue();

	for (unsigned long n = 0; n < Module::framesPerBlock; n++) {
			*out++ = value;
	}
}

