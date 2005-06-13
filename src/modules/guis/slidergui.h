#ifndef SLIDERGUI_H
#define SLIDERGUI_H

#include "modulegui.h"
#include "guislider.h"
#include "../slider.h"

/**
 * GUI modulu Slider.
 */
class SliderGui : public ModuleGui
{
	public:
		SliderGui(Slider* slidermod);
		~SliderGui();
		
	private:
		GuiSlider	guiSlider;
		Slider*		sliderModule;
};

#endif // SLIDERGUI_H
