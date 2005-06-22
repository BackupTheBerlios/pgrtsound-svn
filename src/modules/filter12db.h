#ifndef FILTER12DB_H
#define FILTER12DB_H

#include <cmath>

#include "module.h"

//class ParameterFilterType : public ParameterString {
//	void SetText(string newFilteType);
//}
//
//
//void ParameterFilterType::SetText(string newFilteType) {
//
//}

/**
 * Filtr dolnoprzepustowy.
 Moduly reazliuje filtracje DP z wykorzystaniem cyfroewj imlpementacji
 filtru Butterwotha??? (filtr otrzymnae prez transformacje bulbiowa).
 */
class Filter12dB : public Module
{
	public:
		Filter12dB();
		~Filter12dB();
		void Process();
		void Init();
		static Module* Create();
		
	private:
		Input iIn;
		Output oOut;
		ParameterFloat pFreq;
		//ParameterString pFreq;
		float a0, a1, a2, b0, b1, b2, a0inv;
		float omega, frequency, K, Q, V;
		float xn, yn, z1n, z1m, z2n, z2m;
};

#endif // FILTER12dB_H
