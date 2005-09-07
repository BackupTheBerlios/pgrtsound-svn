#ifndef MYGTKSLIDER_H
#define MYGTKSLIDER_H

#include <gtkmm/scale.h>
#include <gtkmm/adjustment.h>

#include "../../modules/module_parameters.h"
#include "../../debug.h"

/**
 * Suwak dla GUI modulu.
 * Klasa umozliwia szybkie doddwanie typowych suwakow do GUI modulow.
 */
class MyGtkSlider : public Gtk::HScale {
	public:
		MyGtkSlider(double min, double max, double step);
		~MyGtkSlider();
		
		void SetParameter(ParameterFloat* param, double min, double max, double step );
		void ValueChanged();
		void ChangeRange( double min, double max, double step );

	private:
        ParameterFloat*	parameter;
		Gtk::Adjustment	adj;
};

#endif // MYGTKSLIDER_H
