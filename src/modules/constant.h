#ifndef CONSTANT_H
#define CONSTANT_H

#include "module.h"

/**
 * No description
 */
class Constant : public Module {
	public:
		Constant();
		~Constant();
		void Process();
		
	private:
		int oValue;	// indeks wyjscia
		int pValue; // indeks parametru
		float oldValue;
		
};

#endif // CONSTANT_H
