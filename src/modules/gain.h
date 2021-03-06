#ifndef GAIN_H
#define GAIN_H

#include "module.h"

/**
 Modul wzmocnienia.
 Reguleje wzmocnienie 
*/
class Gain : public Module
{
	public:
		Gain();
		~Gain();
		void Process();
		REGISTER_MODULE( "Gain", Gain )
		
	private:
		Input iIn;
		Output oOut;
		ParameterFloat pGain;
};

#endif // MUL_H
