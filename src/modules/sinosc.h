#ifndef SINOSC_H
#define SINOSC_H

#include "module.h"

/**
 * Prosty oscylator sinusoidalny
 */
class SinOsc : public Module
{
	public:
		SinOsc();
		~SinOsc();
		void Process();

	private:
		int iFreq, iAmp, oOut; // indeksy wejsc i wyjsc
		float phase, phaseIncrement;
		int n;
};

#endif // SINOSC_H
