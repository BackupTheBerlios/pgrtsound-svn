#include "slider.h"
#include "guis/slidergui.h"

// class constructor
Slider::Slider() : Module("slider", "New Slider"),
	oOut("wy"), pMin("min", gtProperty), pMax("max", gtProperty), pValue("value", gtParameter)
{
	AddOutput(&oOut);
	AddParameter(&pMin);
	AddParameter(&pMax);
	AddParameter(&pValue);
}


Slider::~Slider() {
//   	TRACE3("Slider::~Slider()", GetName(), " Sprzatam GUI: ", gui);
//   	delete gui;
//  	gui = NULL;
}

void Slider::Init() {
   	pValue.Bound(pMin.GetValue(), pMax.GetValue(), 0.1f);
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

ModuleGui* Slider::GetGui() {
	if(gui == NULL) {
		gui = new SliderGui(this);
	}
	TRACE("Slider::GetGui()", gui);
	return gui;
}
