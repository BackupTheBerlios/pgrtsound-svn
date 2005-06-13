#include "slidergui.h"

SliderGui::SliderGui(Slider* slidermod) : guiSlider(0, 1, 0.1) {

	TRACE("    SliderGui::SliderGui()", "Konstruuuje...");
	sliderModule = slidermod;
	ParameterFloat* param = (ParameterFloat*)sliderModule->GetParameter(2);

	guiSlider.SetParameter(param);
	guiSlider.ChangeRange(param->GetMin(), param->GetMax(), 0.1f);
	cout << sliderModule->GetName() << "." << param->GetName() << " " << param->GetValue() << endl;
	guiSlider.set_value( param->GetValue() );

	add(guiSlider);

	TRACE("    SliderGui::SliderGui()", "Utworzony");
}

SliderGui::~SliderGui() {
	TRACE("SliderGui::~SliderGui()", "Koniec");
}
