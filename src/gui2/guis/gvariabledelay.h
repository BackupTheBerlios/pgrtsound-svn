#ifndef GVARIABLEDELAY_H
#define GVARIABLEDELAY_H

#include "gui.h"
#include "mygtkslider.h"
#include "mygtkfloatentry.h"
#include "../../modules/mvariabledelay.h"

#include <gtkmm/box.h>
#include <gtkmm/label.h>

/**
 * No description
 */
class GVariableDelay : public Gui
{
	public:
		GVariableDelay( Module* mod );
		~GVariableDelay();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( MVariableDelay::GetTypeStatic(), GVariableDelay )
		void OnDelayChanged();
		
	protected:
		MyGtkSlider slFeedback;
		MyGtkFloatEntry delayEntry;
		Gtk::Button delayButton;
		Gtk::VBox box;
		Gtk::HBox delayBox;
		Gtk::Label delayLabel;
};

#endif // GVARIABLEDELAY_H
