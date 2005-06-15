#include "guimodule.h"

GuiModule::GuiModule(Module* moduleToAttach) {
   	module = moduleToAttach;
	x = 0;
	y = 0;
}

GuiModule::~GuiModule() {
}

Gtk::Widget* GuiModule::GetGui() {
	return NULL;
}
