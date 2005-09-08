#ifndef CONSTANT_H
#define CONSTANT_H

#include "module.h"

/**
 * Modul liczbowej wartosci stalej.
 * Na wyjsciu modul pojawia sie non stop ustalona wartosc.
 */
class Constant : public Module {
	public:
		Constant();
		~Constant();
		virtual void Process();
		REGISTER_MODULE( "Constant", Constant )
		
	private:
        float oldValue;
 		Output oValue;
		ParameterFloat pValue;
};

#endif // CONSTANT_H
