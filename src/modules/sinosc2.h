#ifndef SINOSC2_H
#define SINOSC2_H

#include <math.h>

#include "module.h"

/**
 Oscylator sisnusoidalny.
 Prosty tablicoyw oscylator o sinusoidalnym ksztlacie fali. Zakres i skos
 czestoltiowsc oscylatora jest ograniczony wymiarami tablicy.

 - Wejscia
    -# freq Zadaje czestotliowsc [Hz] oscylatora
    -# ampl Decyduje o amplitudzie przebiegu wyjsciowego
 - Wyjscia
    -# out Przebieg sinusoidalny o wartosciach z przedzialu [-1; 1]
*/
class SinOsc2 : public Module {
	public:
		SinOsc2();
		~SinOsc2();
		void Process();
		static Module* Create();
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
