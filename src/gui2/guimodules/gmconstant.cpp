#include "gmconstant.h" // class's header file

GMConstant::GMConstant( Module* module ) :
	GuiModule( module ),
	gui( module )
{
	cout << module->GetParameter(0) << endl;
}

GMConstant::~GMConstant() {
	// insert your code here
}

Gtk::Widget* GMConstant::GetGui() {
	return &gui;
}
