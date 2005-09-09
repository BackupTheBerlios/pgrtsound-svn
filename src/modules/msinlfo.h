#ifndef MSINLFO_H
#define MSINLFO_H

#include "module.h"

/**
 Sinusoidalny oscylator niskich czestotliwosci.
 */
class MSinLFO : public Module {
	public:
		MSinLFO();
		~MSinLFO();
		virtual void Process();
		virtual void Init();
		REGISTER_MODULE( "LFO Sin", MSinLFO )
		void Update();

	protected:
		ParameterFloat pFreq;
		Output oOut;
		float sinZ, cosZ, freq;
};

#endif // MSINLFO_H
