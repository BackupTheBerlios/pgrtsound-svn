#include "CConnector.h"

#ifndef CBLOCK_H
#define CBLOCK_H

class CBlock{
public:

    //class constructor
	CBlock();
	//class destructor
	virtual ~CBlock();
	
	//parametry sytemowe
	int BUFFOR_SIZE;
	
	//druty
	CConnector *inConnection;
	int   *inputCount;
	
	CConnector *outConnection;
	int   *outputCount;
	
	//parametry wej�ciowe
	float *param;
	int   *paramCount;
	
	//funkcjia obliczaj�ca/generuj�ca
	virtual void Process(void);
	
	CBlock* AddrOfNextBlock;
	CBlock* AddrOfPrevBlock;
};

#endif //CBLOCK_H
