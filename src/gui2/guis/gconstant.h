#ifndef GMCONSTANT_H
#define GMCONSTANT_H

#include "gui.h"
#include "mygtkfloatentry.h"
#include "../../modules/constant.h"

/*
 * No description
 */
class GConstant : public Gui
{
	public:
		GConstant( Module* mod );
		~GConstant();
		Gtk::Widget* GetGui();
		REGISTER_GUI( Constant::GetTypeStatic(), GConstant )

	protected:
		MyGtkFloatEntry entry;
};

#endif // GMCONSTANT_H
