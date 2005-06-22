#ifndef GUIMODULE_H
#define GUIMODULE_H

#include <gtkmm/widget.h>

#include "../../modules/module.h"

/**
 * Interfejs modulow dla graficznego interfejsu uzytkownika.
 */
class GuiModule {
	public:
		GuiModule(Module* moduleToAttach);
		~GuiModule();
		virtual Gtk::Widget* GetGui();
		int GetX();
		int GetY();
		void SetXY(int x_, int y_);
		Module* GetModule();

	private:
		Module* module;
		int		x, y;
};

#endif // GUIMODULE_H
