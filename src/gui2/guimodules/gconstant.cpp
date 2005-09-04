#include "gconstant.h"

GConstant::GConstant( Module* mod ) : Gui( mod ) {
	cout << "GConstant::GConstant" << endl;
	entry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );
}

GConstant::~GConstant() {
	// insert your code here
}

Gtk::Widget* GConstant::GetGui() {
	cout << "GConstant::GetGui" << endl;
	return &entry;
}
