#ifndef GSINLFO_H
#define GSINLFO_H

#include "gui.h"
#include "mygtkslider.h"
#include "../../modules/msinlfo.h"

#include <gtkmm/button.h>

/**
 No description
*/
class GSinLFO : public Gui
{
	public:
		GSinLFO( Module* mod );
		~GSinLFO();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( MSinLFO::GetTypeStatic(), GSinLFO )

	protected:
		MyGtkSlider sliderFreq;
		MSinLFO* lfoModule;
};

#endif // GSINLFO_H
