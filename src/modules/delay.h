#ifndef DELAY_H
#define DELAY_H

#include "module.h"

/**
 Modul opó?nienia.
 Reguleje opó?nienie 
*/
class Delay : public Module
{
	public:
		Delay();
		~Delay();
		void Process();
		REGISTER_MODULE( "Delay", Delay )
		
	private:
		Input           iIn;
		Output          oOut;
		ParameterFloat  pDelay;
		float           *buffor;
		int             n1, n2;
		int             maxDelay;
};

#endif // DELAY_H
