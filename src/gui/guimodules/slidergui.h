#ifndef SLIDERGUI_H
#define SLIDERGUI_H

#include "guimodule.h"
#include "mygtkslider.h"

/**
 * Interfejs graficzny dla modulu slider
 */
class SliderGui : public GuiModule
{
	public:
		SliderGui(Module* module);
		~SliderGui();
		Gtk::Widget* GetGui();
		
	private:
		MyGtkSlider	guiSlider;
};

#endif // SLIDERGUI_H
