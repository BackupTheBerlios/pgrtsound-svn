#ifndef GMCONSTANT_H
#define GMCONSTANT_H

#include "gui.h"
#include "mygtkfloatentry.h"
#include "../../modules/constant.h"

#include <gtkmm/button.h>
#include <gtkmm/box.h>

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
		void OnApply();

	protected:
        MyGtkFloatEntry entry;
        Gtk::HBox box;
		Gtk::Button buttonApply;
};

#endif // GMCONSTANT_H
