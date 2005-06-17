#ifndef GUIMODULE_H
#define GUIMODULE_H

#include <gtkmm/widget.h>

#include "../../modules/module.h"

/**
 * Interfejs modulow dla graficznego interfejsu uzytkownika
 */
class GuiModule {
	public:
		GuiModule(Module* moduleToAttach);
		~GuiModule();
		virtual Gtk::Widget* GetGui();

	private:
		Module* module;
		int		x, y;
};

#endif // GUIMODULE_H
