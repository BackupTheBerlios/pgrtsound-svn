#ifndef GSLIDER_H
#define GSLIDER_H

#include "gui.h"
#include "mygtkslider.h"
#include "../../modules/slider.h"

/**
 * No description
 */
class GSlider : public Gui
{
	public:
		GSlider( Module* mod );
		~GSlider();
		Gtk::Widget* GetGui();
		REGISTER_GUI( Slider::GetTypeStatic(), GSlider )

	protected:
		MyGtkSlider	guiSlider;
};

#endif // GSLIDER_H
