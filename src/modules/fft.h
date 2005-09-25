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
		float           *buffor;
		
		unsigned int    n;
		unsigned int    i;
};

#endif // FFT_H
