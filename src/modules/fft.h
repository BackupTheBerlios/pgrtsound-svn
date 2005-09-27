#ifndef FFT_H
#define FFT_H

#include "module.h"
#include "fftw++.h"
#include <deque>

/**
 Modul opóŸnienia.
 Reguleje opóŸnienie 
*/
class FFT : public Module
{
	public:
		FFT();
		~FFT();
		void Process();
		REGISTER_MODULE( "FFT", FFT )
		
	private:
		Input           iIn;
		Output          oReal;
		Output          oImag;
		ParameterFloat  pSize;
		float           *bufforIn;
		float           *bufforOut1;
		float           *bufforOut2;    		
		
		unsigned int    n;
		unsigned int    i;
		unsigned int    o;
};

#endif // FFT_H
