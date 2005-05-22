#include "guiparameterslider.h"

GuiParameterSlider::GuiParameterSlider(double min, double max, double step) :
    adj(0, min, max, step, step, 0)
{
	set_adjustment(adj);
	set_update_policy(Gtk::UPDATE_CONTINUOUS);
	set_digits(2);
}

GuiParameterSlider::~GuiParameterSlider() {
	TRACE("GuiParameterSlider::~GuiParameterSlider()", "Usuniety");
}

void GuiParameterSlider::SetValue() {
	parameter->SetValue(get_value());
}

void GuiParameterSlider::BindParameterFloat(ParameterFloat* param) {
    parameter = param;
}
