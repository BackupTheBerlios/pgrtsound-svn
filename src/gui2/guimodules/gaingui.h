#ifndef GAINGUI_H
#define GAINGUI_H

#include "guimodule.h"
#include "mygtkslider.h"

/*
 * GUI modulu Gain.
 */
class GainGui : public GuiModule
{
	public:
		GainGui(Module* module);
		~GainGui();
		Gtk::Widget* GetGui();
		
	private:
        MyGtkSlider	guiSlider;

};

#endif // GAINGUI_H
