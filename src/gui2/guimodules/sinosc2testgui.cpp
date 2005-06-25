#include "sinosc2testgui.h"

SinOsc2TestGui::SinOsc2TestGui(Module* module) : GuiModule(module) {
	label.set_text("Halo! To ja SinOsc. Tak naprawde nie mam GUI.\n"
	   "To tylko marny pokaz!!!");
}

SinOsc2TestGui::~SinOsc2TestGui() {
}

Gtk::Widget* SinOsc2TestGui::GetGui() {
	return &label;
}
