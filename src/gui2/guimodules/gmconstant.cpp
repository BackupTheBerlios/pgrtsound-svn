#include "gmconstant.h" // class's header file

GMConstant::GMConstant( Module* module ) :
	GuiModule( module )
{
	entry.AttachFloatParameter( (ParameterFloat*)module->GetParameter( 0 ) );
}

GMConstant::~GMConstant() {
	// insert your code here
}

Gtk::Widget* GMConstant::GetGui() {
	return &entry;
}
