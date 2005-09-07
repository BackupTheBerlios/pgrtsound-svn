#ifndef GSLIDER_H
#define GSLIDER_H

#include "gui.h"
#include "mygtkslider.h"
#include "mygtkfloatentry.h"
#include "../../modules/slider.h"

#include <gtkmm/button.h>
#include <gtkmm/table.h>
#include <gtkmm/label.h>

/**
 * No description
 */
class GSlider : public Gui
{
	public:
		GSlider( Module* mod );
		~GSlider();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( Slider::GetTypeStatic(), GSlider )

	protected:
		MyGtkSlider	guiSlider;
		MyGtkFloatEntry entryMin, entryMax;
		Gtk::Button buttonApply;
		Gtk::Table table;
		Gtk::Label labelMin, labelMax;
		ParameterFloat* pMin, * pMax;
		// No description
		void OnApply();
};

#endif // GSLIDER_H
