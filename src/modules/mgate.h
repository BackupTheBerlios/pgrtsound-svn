#ifndef MGATE_H
#define MGATE_H

#include "module.h"

/**
 * No description
 */
class MGate : public Module {
	public:
		MGate();
		~MGate();
		REGISTER_MODULE( "gate", MGate )
		//virtual void Process();
		virtual void Init();
		void Switch();
		
	protected:
		Output oOut;
		bool enabled;
};

#endif // MGATE_H
