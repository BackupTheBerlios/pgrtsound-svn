#ifndef GDELAY_H
#define GDELAY_H

#include "gui.h"
#include "mygtkfloatentry.h"
#include "../../modules/delay.h"

#include <gtkmm/button.h>
#include <gtkmm/box.h>

/*
 * No description
 */
class GDelay : public Gui
{
	public:
		GDelay( Module* mod );
		~GDelay();
		Gtk::Widget* GetGui();
		REGISTER_GUI( Delay::GetTypeStatic(), GDelay )
		void OnApply();

	protected:
        MyGtkFloatEntry entry;
        Gtk::HBox box;
		Gtk::Button buttonApply;
};

#endif // GDELAY_H