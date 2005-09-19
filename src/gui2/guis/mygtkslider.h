#ifndef MYGTKSLIDER_H
#define MYGTKSLIDER_H

#include "../../modules/module_parameters.h"
#include "../../debug.h"

#include <gtkmm/scale.h>
//#include <gtkmm/adjustment.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <string>


/**
 * Suwak dla GUI modulu.
 * Klasa umozliwia szybkie doddwanie typowych suwakow do GUI modulow.
 */
class MyGtkSlider : public Gtk::HBox {
	public:
		//MyGtkSlider(double min, double max, double step);
		MyGtkSlider();
		~MyGtkSlider();
		
		void SetParameter(ParameterFloat* param, double min, double max, double step_ );
		void ValueChanged();
		void ChangeRange( double min, double max, double step );
		void SetCaption(string value);
		// nowy sygnal
		typedef sigc::signal<void> type_signal_slider_moved;
		type_signal_slider_moved signal_slider_moved();

	private:
        type_signal_slider_moved m_signal_slider_moved;
        ParameterFloat*	parameter;
        Gtk::HScale slider;
        Gtk::Label label;
        double  step;
		//Gtk::Adjustment	adj;
};

#endif // MYGTKSLIDER_H
