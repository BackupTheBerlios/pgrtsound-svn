#ifndef SLIDER_H
#define SLIDER_H

#include "module.h"

/**
 * No description
 */
class Slider : public Module
{
	public:
		Slider();
		~Slider();
		void Process();
		void Init();
		
	protected:
		Output			oOut;
		ParameterFloat	pMin;
		ParameterFloat	pMax;
		ParameterFloat	pValue;
};

#endif // SLIDER_H
