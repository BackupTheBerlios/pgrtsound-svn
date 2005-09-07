#include "mmonitor.h"

MMonitor::MMonitor() :
	Module("New Monitor"),
	iIn( "monitor input" )
{
	AddInput( iIn );
}


MMonitor::~MMonitor() {
}
