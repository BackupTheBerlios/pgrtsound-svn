#ifndef DELAY_H
#define DELAY_H

#include "module.h"

/**
 Modul opóŸnienia.
 Reguleje opóŸnienie 
*/
class Delay : public Module
{
	public:
		Delay();
		~Delay();
		void Process();
		static Module* Create();
		static string GetTypeStatic();
		string GetType();
		
	private:
		Input           iIn;
		Output          oOut;
		ParameterFloat  pDelay;
		float           *buffor;
		int             n1, n2;
		int             maxDelay;
};

#endif // MUL_H
