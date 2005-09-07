#ifndef GGAIN_H
#define GGAIN_H

#include "gui.h"
#include "../../modules/gain.h"
#include "mygtkslider.h"


/**
 * No description
 */
class GGain : public Gui
{
	public:
		GGain( Module* mod );
		~GGain();
		Gtk::Widget* GetGui();
		REGISTER_GUI( Gain::GetTypeStatic(), GGain )

	protected:
		MyGtkSlider slider;
};

#endif // GGAIN_H
