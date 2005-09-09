#include "gsinlfo.h"


GSinLFO::GSinLFO( Module* mod ) :
	Gui( mod )
{
    lfoModule = (MSinLFO*)module;
	sliderFreq.SetParameter( (ParameterFloat*)lfoModule->GetParameter( 0 ),
		1, 200, 0.1 );
		
	sliderFreq.signal_slider_moved().connect(
		sigc::mem_fun( lfoModule, &MSinLFO::Update ) );
}

GSinLFO::~GSinLFO() {
}

Gtk::Widget* GSinLFO::GetGui() {
	return &sliderFreq;
}
