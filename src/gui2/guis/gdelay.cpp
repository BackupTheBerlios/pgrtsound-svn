#include "gdelay.h"

GDelay::GDelay( Module* mod ) : Gui( mod ) {

    
	slider.SetParameter( (ParameterFloat*)module->GetParameter(0), 0, 40000, 0 );
    slider.SetCaption( "OpóŸnienie [próbki]" );
	delayBox.set_spacing( 2 );
	delayBox.add( slider );


}

GDelay::~GDelay() {
}

Gtk::Widget* GDelay::GetGui() {
	return &delayBox;
}

/*
void GDelay::OnApply() {
	entry.Update();
}
*/
