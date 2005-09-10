#ifndef MMONITORGRAPH_H
#define MMONITORGRAPH_H

#include "module.h"

/**
 Modul bazowy dla monitora graficznego. Monitor to modul przydatny w GUI. Wyswietla
 wartosci podawane na jego wejscie w formie wykresu.

 */
class MMonitorGraph : public Module
{
	public:
		MMonitorGraph();
		~MMonitorGraph();
		REGISTER_MODULE( "Graph Monitor", MMonitorGraph )
	protected:
		Input iIn;
};

#endif // MMONITORGraph_H
