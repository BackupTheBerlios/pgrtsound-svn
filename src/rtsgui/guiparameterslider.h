#ifndef GUIPARAMETERSLIDER_H
#define GUIPARAMETERSLIDER_H

#include <gtkmm.h>

#include "algorithm.h"

/*
 * No description
 */
class GuiParameterSlider : public Gtk::HScale
{
	public:
		GuiParameterSlider(double min, double max, double step);
		~GuiParameterSlider();
		void SetValue();
		void BindParameterFloat(ParameterFloat* param);
		
	protected:
   		ParameterFloat*	parameter;
		Gtk::Adjustment	adj;
};

#endif // GUIPARAMETERSLIDER_H
