#include "gaingui.h" 

GainGui::GainGui(Module* module) : GuiModule(module), guiSlider(0, 1, 0.1)
{
	ParameterFloat* param = (ParameterFloat*)module->GetParameter(0); // parametr gain
	guiSlider.SetParameter(param);
	guiSlider.ChangeRange(param->GetMin(), param->GetMax(), 0.1f);
	guiSlider.set_value( param->GetValue() );
}

GainGui::~GainGui() {
}

Gtk::Widget* GainGui::GetGui() {
	return &guiSlider;
}
