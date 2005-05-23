#ifndef SINOSC2_H
#define SINOSC2_H

#include "module.h" // inheriting class's header file

/**
 * No description
 */
class SinOsc2 : public Module {
	public:
		SinOsc2();
		~SinOsc2();
		void Process();
		
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
