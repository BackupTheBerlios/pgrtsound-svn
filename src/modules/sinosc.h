#ifndef SINOSC_H
#define SINOSC_H

#include <math.h>

#include "module.h"

/**
 * Prosty, tablicowy oscylator sinusoidalny
 */
class SinOsc : public Module
{
	public:
		SinOsc();
		~SinOsc();
		void process();

	private:
		int iFreq, iAmp, oOut; // indeksy wejsc i wyjsc
		float phase, phaseIncrement;
};

#endif // SINOSC_H
