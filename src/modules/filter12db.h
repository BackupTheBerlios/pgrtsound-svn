#ifndef FILTER12DB_H
#define FILTER12DB_H

#include "module.h"

#include <cmath>

/**
 Filtr dolno- i gornoperzepustowy.
 Moduly reazliuje filtracje z wykorzystaniem cyfroewj imlpementacji
 filtrow otrzymnye przez transformacje biliniowa analogowyhc prototypow
 dwubiegunowych.
*/
class Filter12dB : public Module {
	public:
		Filter12dB();
		~Filter12dB();
		virtual void Process();
		virtual void Init();
		REGISTER_MODULE( "Filter LP/HP", Filter12dB )
		void UpdateCoefficients();
				
	protected:
		Input iIn;
		Output oOut;
		ParameterFloat pFreq;
		ParameterString pFilterType;
		float a0, a1, a2, b0, b1, b2, A1, A2, B0, B1, B2;
		float w0, Q;
	    float cos_w0, sin_w0, alpha;
		float xn, yn, xn_1, xn_2, yn_1, yn_2;
};

#endif // FILTER12dB_H
