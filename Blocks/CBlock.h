#include "CConnector.h"

#ifndef CBLOCK_H
#define CBLOCK_H

class CBlock{
public:
  //Parametry / atrybuty

	//parametry sytemowe
	int BUFFOR_SIZE;
	
	//druty
	CConnector *inConnection;
	int   *inputCount;
	
	CConnector *outConnection;
	int   *outputCount;
	
	//parametry sta�e
	float *param;
	int   *paramCount;



    //class constructor
	CBlock();
	//class destructor
	virtual ~CBlock();
	
	
	//funkcjia obliczaj�ca/generuj�ca
	virtual void Process(void);
	
};

#endif //CBLOCK_H
