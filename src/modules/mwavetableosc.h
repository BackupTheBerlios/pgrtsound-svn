#ifndef WAVETABLEOSC_H
#define WAVETABLEOSC_H

#include <math.h>

#include "module.h"

/**
 Oscylator tablicowy.
 Prosty tablicoyw oscylator o sinusoidalnym ksztlacie fali. Zakres
 czestoltiowsc oscylatora jest ograniczony wymiarami tablicy.

- Wejscia
	- [0] \e freq Zadaje czestotliowsc [Hz] oscylatora
	- [1] \e ampl Decyduje o amplitudzie przebiegu wyjsciowego
- Wyjscia
	- [0] \e out Przebieg audio o wartosciach z przedzialu [-1; 1]
*/
class MWavetableOsc : public Module {
	public:
		MWavetableOsc();
		~MWavetableOsc();
		void Process();
		REGISTER_MODULE( "Wavetable Oscillator", MWavetableOsc )

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

#endif // WAVETABLEOSC_H
