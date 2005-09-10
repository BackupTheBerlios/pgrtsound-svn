#include "mmonitorgraph.h"

MMonitorGraph::MMonitorGraph() :
	Module("New Graph Monitor"),
	iIn( "graph monitor input" )
{
	AddInput( iIn );
}


MMonitorGraph::~MMonitorGraph() {
}
