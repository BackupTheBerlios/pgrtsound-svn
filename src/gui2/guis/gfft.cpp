#include "gfft.h"

GFFT::GFFT( Module* mod ) : Gui( mod ) {
    
	entry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );
	entry.SetValue(256);
	buttonApply.set_label( Glib::locale_to_utf8("Zmieñ wartoœæ") );
	buttonApply.signal_clicked().connect( sigc::mem_fun( this, &GFFT::OnApply ) );
	
	box.set_spacing( 2 );
	box.add( entry );
	box.add( buttonApply );
}

GFFT::~GFFT() {
	// insert your code here
}

Gtk::Widget* GFFT::GetGui() {
	return &box;
}

void GFFT::OnApply() {
	entry.Update();
}
