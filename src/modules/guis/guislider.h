#ifndef GUISLIDER_H
#define GUISLIDER_H

#include <gtkmm.h>

#include "../module_parameters.h"
#include "../../debug.h"

/**
 * Suwak dla GUI modulu.
 * Klasa umozliwia szybkie doddwanie typowych suwakow do GUI modulow.
 */
class GuiSlider : public Gtk::HScale {
	public:
		GuiSlider(double min, double max, double step);
		~GuiSlider();
		void SetParameter(ParameterFloat* param);
		void ValueChanged();
		void ChangeRange(double min, double max, double step);
		
	private:
        ParameterFloat*	parameter;
		Gtk::Adjustment	adj;
};

#endif // GUISLIDER_H
