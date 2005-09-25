#ifndef MModule_H
#define MModule_H

#include "module.h"

/**
 * No description
 */
class MModule : public Module {
	public:
		MModule();
		~MModule();
		void Process();
		REGISTER_MODULE( "Modul", MModule )
		
	private:
		Input i1, i2;
		Output oModule;
};

#endif // MModule_H
