#ifndef GFEEDBACKDELAY_H
#define GFEEDBACKDELAY_H

#include "gui.h"
#include "mygtkslider.h"
#include "mygtkfloatentry.h"
#include "../../modules/mfeedbackdelay.h"

#include <gtkmm/box.h>
#include <gtkmm/label.h>

/**
 * No description
 */
class GFeedbackDelay : public Gui
{
	public:
		GFeedbackDelay( Module* mod );
		~GFeedbackDelay();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( MFeedbackDelay::GetTypeStatic(), GFeedbackDelay )
		void OnDelayChanged();
		
	protected:
		MyGtkSlider slFeedback;
		MyGtkFloatEntry delayEntry;
		Gtk::Button delayButton;
		Gtk::VBox box;
		Gtk::HBox delayBox;
		Gtk::Label delayLabel;
};

#endif // GFEEDBACKDELAY_H
