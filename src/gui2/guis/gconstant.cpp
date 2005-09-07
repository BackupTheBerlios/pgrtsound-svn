#include "gconstant.h"

GConstant::GConstant( Module* mod ) : Gui( mod ) {
	entry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );
	
	buttonApply.set_label( Glib::locale_to_utf8("Zmieñ wartoœæ") );
	buttonApply.signal_clicked().connect( sigc::mem_fun( this, &GConstant::OnApply ) );
	
	box.set_spacing( 2 );
	box.add( entry );
	box.add( buttonApply );
}

GConstant::~GConstant() {
	// insert your code here
}

Gtk::Widget* GConstant::GetGui() {
	return &box;
}

void GConstant::OnApply() {
	entry.Update();
}
