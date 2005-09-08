#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H

#include "module.h"

/**
 * Mnozenie.
 */
class Multiplication : public Module
{
	public:
		Multiplication();
		~Multiplication();
		void Process();
		REGISTER_MODULE( "Multiplication", Multiplication )
		
	protected:
		Input   iIn1, iIn2;
		Output  oOut;
};

#endif // MULTIPLICATION_H
