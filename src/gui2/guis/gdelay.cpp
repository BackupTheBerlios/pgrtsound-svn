#include "gdelay.h"

GDelay::GDelay( Module* mod ) : Gui( mod ) {
	entry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );
	
	buttonApply.set_label( Glib::locale_to_utf8("Zmieñ wartoœæ") );
	buttonApply.signal_clicked().connect( sigc::mem_fun( this, &GDelay::OnApply ) );
	
	box.set_spacing( 2 );
	box.add( entry );
	box.add( buttonApply );
}

GDelay::~GDelay() {
}

Gtk::Widget* GDelay::GetGui() {
	return &box;
}

void GDelay::OnApply() {
	entry.Update();
}
