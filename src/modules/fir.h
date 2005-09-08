#ifndef FIR_H
#define FIR_H

#include <cmath>

#include "module.h"

/**
 * FIR 
 */
class FIR : public Module
{
	public:
		FIR();
		~FIR();
		void Process();
		void Init();
		
		REGISTER_MODULE( "fir", FIR )
				
	private:
		Input iIn;
		Output oOut;
	
		ParameterFloat b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
		
        float *buffor;
        int   n;
};

#endif // FIR
