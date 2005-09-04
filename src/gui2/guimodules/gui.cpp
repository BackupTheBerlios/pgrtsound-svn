#include "gui.h"

Gui::Gui( Module* mod ) {
	module = mod;
}

Gui::~Gui() {
}

Gtk::Widget* Gui::GetGui() {
	return NULL;
}
