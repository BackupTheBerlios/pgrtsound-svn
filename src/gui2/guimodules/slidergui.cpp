#include "slidergui.h"

SliderGui::SliderGui(Module* module) : GuiModule(module),
	guiSlider(0, 1, 0.1) {
	//TRACE("    SliderGui::SliderGui()", "Konstruuuje...");
	ParameterFloat* param = (ParameterFloat*)module->GetParameter(2); // parametr value
	guiSlider.SetParameter(param);
	guiSlider.ChangeRange(param->GetMin(), param->GetMax(), 0.1f);
	//	cout << sliderModule->GetName() << "." << param->GetName() << " " << param->GetValue() << endl;
	guiSlider.set_value( param->GetValue() );
	//TRACE("    SliderGui::SliderGui()", "Utworzony");
}

SliderGui::~SliderGui() {
	// insert your code here
}

Gtk::Widget* SliderGui::GetGui() {
	return &guiSlider;
}
