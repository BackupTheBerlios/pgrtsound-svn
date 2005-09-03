#ifndef SINOSC2_H
#define SINOSC2_H

#include <math.h>

#include "module.h"

/**
 Oscylator sisnusoidalny.
 Prosty tablicoyw oscylator o sinusoidalnym ksztlacie fali. Zakres i skos
 czestoltiowsc oscylatora jest ograniczony wymiarami tablicy.

- Wejscia
	- [0] \e freq Zadaje czestotliowsc [Hz] oscylatora
	- [1] \e ampl Decyduje o amplitudzie przebiegu wyjsciowego
- Wyjscia
	- [0] \e out Przebieg sinusoidalny o wartosciach z przedzialu [-1; 1]
*/
class SinOsc2 : public Module {
	public:
		SinOsc2();
		~SinOsc2();
		void Process();
		REGISTER_MODULE( "sinosc2", SinOsc2 )

	private:
		Input	iFreq, iAmp;
		Output	oOut;

		long	tablePosition, tableRingMask, tableSize;
		float	phase, phaseFraction;
		float 	phaseFloor;
		float*	sinTable;	// adres tablicy probek
		float	*tempSample;
		float	scale;
};

#endif // SINOSC2_H
