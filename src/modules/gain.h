#ifndef GAIN_H
#define GAIN_H

#include "module.h"

/**
 * Modul mnozenia. Mnozy skalarnie dwa sygnaly wejsciowe.
 */
class Gain : public Module
{
	public:
		Gain();
		~Gain();
		void Process();
	
	private:
		int iIn, oOut, pGain;
};

#endif // MUL_H
