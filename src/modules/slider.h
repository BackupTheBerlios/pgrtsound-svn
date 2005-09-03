#ifndef SLIDER_H
#define SLIDER_H

#include "module.h"

/**
 Modul suwaka GUI.
 Suwak widoczny w GUI decyduje o wartosci widczone jna wyjsciu tego modulu.
 Ustaleniu podlega przedzial, z ktorego wartosci mozlwie sa do ustawienia.

- wyjscia
	 -# value Widoczna ustlaona polozeniem suwaka wartosc
*/
class Slider : public Module
{
	public:
		Slider();
		~Slider();
		void Process();
		void Init();
		REGISTER_MODULE( "slider", Slider )

	protected:
		Output			oOut;
		ParameterFloat	pMin;
		ParameterFloat	pMax;
		ParameterFloat	pValue;
};

#endif // SLIDER_H
