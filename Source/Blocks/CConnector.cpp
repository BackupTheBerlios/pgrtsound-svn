#include "CConnector.h"


CConnector::CConnector(int bufferSize)
{
	buffer = new float[bufferSize];
}

CConnector::~CConnector()
{
	delete buffer;
}

// bez sensu
float *CConnector::In(void)
{
	return buffer;	
}

// bez sensu
float *CConnector::Out(void)
{
	return buffer;
}
