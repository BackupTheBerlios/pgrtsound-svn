#ifndef GFILTER_H
#define GFILTER_H

#include "gui.h"
#include "mygtkfloatentry.h"
#include "../../modules/filter12db.h"

#include <gtkmm/radiobutton.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>

/**
 * No description
 */
class GFilter : public Gui
{
	public:
		GFilter( Module* mod );
		~GFilter();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( Filter12dB::GetTypeStatic(), GFilter )
		void OnApply();
		
	protected:
        ParameterString* pFilerType;
		MyGtkFloatEntry freqEntry;
		Gtk::VBox mainBox;
		Gtk::HBox freqBox;
		Gtk::Label labelFreq;
		Gtk::Button buttonApply;
        Gtk::RadioButton::Group group;
        Gtk::RadioButton *rbLP, *rbHP;
        
};

#endif // GFILTER_H
