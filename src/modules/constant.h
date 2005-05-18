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
        float oldValue;
        
		Output oValue;
		ParameterFloat pValue;
};

#endif // CONSTANT_H
