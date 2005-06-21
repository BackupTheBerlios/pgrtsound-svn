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

int GuiModule::GetX() {
    return x;
}

int GuiModule::GetY() {
    return y;
}

void GuiModule::SetXY(int x_, int y_) {
    x = x_;
    y = y_;
}

Module* GuiModule::GetModule() {
    return module;
}
