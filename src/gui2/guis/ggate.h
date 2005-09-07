#ifndef GGATE_H
#define GGATE_H

#include "gui.h"
#include "../../modules/mgate.h"

#include <gtkmm/button.h>

class GGate : public Gui {
	public:
		GGate( Module* mod );
		~GGate();
		REGISTER_GUI( MGate::GetTypeStatic(), GGate )
		virtual Gtk::Widget* GetGui();
		void Switch();

	protected:
		Gtk::Button buttonGate;

};

#endif // GGATE_H
