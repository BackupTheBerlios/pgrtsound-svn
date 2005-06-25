#ifndef SINOSC2TESTGUI_H
#define SINOSC2TESTGUI_H

#include <gtkmm/label.h>

#include "guimodule.h" // inheriting class's header file

/**
 * Test GUI na przykladzie SinOsc2.
 */
class SinOsc2TestGui : public GuiModule
{
	public:
		SinOsc2TestGui(Module* module);
		~SinOsc2TestGui();
		Gtk::Widget* GetGui();
		
	private:
		Gtk::Label label;
};

#endif // SINOSC2TESTGUI_H
