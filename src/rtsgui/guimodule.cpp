#include "guimodule.h"

// class constructor
GuiModule::GuiModule() {
	set_label("Modul name");
	//set_border_width(5);

	box.set_border_width(5);
	box.set_homogeneous(false);
	box.set_spacing(0);
    add(box);
}

// class destructor
GuiModule::~GuiModule() {
	TRACE("GuiModule::~GuiModule()", "Modul usuniety");
}

void GuiModule::AddParameter(Parameter* param) {
	TRACE3("GuiModule::AddParameter()", "Dodaje parametr '", param->GetName(), "'...");

	// slider tylko dla floatow
	ParameterFloat* paramf = (ParameterFloat*)param;

	if(param->GetGUIType() == gtSlider) {
		GuiParameterSlider* slider = new GuiParameterSlider(
			paramf->GetMin(),
			paramf->GetMax(),
			paramf->GetStep());

		// ustawienia
		slider->BindParameterFloat(paramf);
		slider->set_value( paramf->GetValue() );
		slider->signal_value_changed().connect( sigc::mem_fun(slider, &GuiParameterSlider::SetValue ));
		
		guiParameters.push_back(slider);
		box.add(*slider);
	}

	TRACE("GuiModule::AddParameter()", "Parametr dodany");
}

void GuiModule::Clear() {
    TRACE("GuiModule::Clear()", "Sprzatam parmetry...");

	for(int i = 0; i < guiParameters.size(); i++) {
		delete guiParameters[i];
	}
	
	guiParameters.clear();
	
	TRACE("GuiModule::Clear()", "Parametry sprzatniete");
}
