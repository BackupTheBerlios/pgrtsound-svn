#ifndef MMONITOR_H
#define MMONITOR_H

#include "module.h"

/**
 Modul bazowy dla monitora. Monitor to modul przydatny w GUI. Wyswietla
 wartosci podawane na jego wejscie.

 */
class MMonitor : public Module
{
	public:
		MMonitor();
		~MMonitor();
		REGISTER_MODULE( "Monitor", MMonitor )
	protected:
		Input iIn;
};

#endif // MMONITOR_H
