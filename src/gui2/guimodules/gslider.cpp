#include "gslider.h"

GSlider::GSlider( Module* mod ) :
	Gui( mod ),
	guiSlider(0, 1, 0.1)
{
	//TRACE("    SliderGui::SliderGui()", "Konstruuuje...");
	ParameterFloat* param = (ParameterFloat*)module->GetParameter(2); // parametr value
	guiSlider.SetParameter( param );
	guiSlider.ChangeRange( param->GetMin(), param->GetMax(), 0.1f );
	//	cout << sliderModule->GetName() << "." << param->GetName() << " " << param->GetValue() << endl;
	guiSlider.set_value( param->GetValue() );
	//TRACE("    SliderGui::SliderGui()", "Utworzony");
}

GSlider::~GSlider() {
	// insert your code here
}

Gtk::Widget* GSlider::GetGui() {
	cout << "GSlider::GetGui" << endl;
	return &guiSlider;
}
