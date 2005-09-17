#ifndef MSINOSC_H
#define MSINOSC_H

#include "module.h"

/**
 Sinusoidalny oscylator.
 */
class MSinOsc : public Module {
	public:
		MSinOsc();
		~MSinOsc();
		virtual void Process();
		virtual void Init();
		virtual void SampleRateChanged();
		REGISTER_MODULE( "Sin Oscillator", MSinOsc )

	protected:
		Input iFreq, iAmp;
		Output oOut;
		
		float sinZ, cosZ, freq, freqCoeff, *amp, *out, *freqIn;
};

#endif // MSINOSC_H
