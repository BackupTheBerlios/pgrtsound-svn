#include "ggain.h"

GGain::GGain( Module* mod) :
	Gui( mod )
{
	slider.SetParameter( (ParameterFloat*)module->GetParameter(0), 0, 1, 0.01 );
}

GGain::~GGain() {
}

Gtk::Widget* GGain::GetGui() {
	return &slider;
}
