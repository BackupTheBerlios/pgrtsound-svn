#ifndef GADSR_H
#define GADSR_H

#include "gui.h"
#include "mygtkslider.h"
#include "../../modules/madsrlinear.h"

#include <gtkmm/box.h>

/**
 * No description
 */
class GADSR : public Gui
{
	public:
		GADSR( Module* mod );
		~GADSR();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( MADSRLinear::GetTypeStatic(), GADSR )
		
	protected:
		Gtk::VBox box;
		MyGtkSlider slAttack, slDecay, slSustain, slRelease;
};

#endif // GADSR_H
