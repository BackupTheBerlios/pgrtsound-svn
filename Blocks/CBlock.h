#include <string>
#include "CConnector.h"

#ifndef CBLOCK_H
#define CBLOCK_H

using namespace std;

class CBlock{
public:
  //Parametry / atrybuty

	//parametry sytemowe
	int BUFFOR_SIZE;
	int idBlock;    //id blokum, unikalny
	string name; 	//Unikalna w skali prokjektu (nazwa moze sie skladac z typ+id ale nie koniecznie
	string type;    //Czy Generator czy Aplife czy inne ustrojstwo :)
	
	
	
	//druty
	CConnector *inConnection;
	int   *inputCount;
	
	CConnector *outConnection;
	int   *outputCount;
	
	//parametry sta³e
	float *param;
	int   *paramCount;



    //class constructor
	CBlock();
	//class destructor
	virtual ~CBlock();
	
	
	//funkcjia obliczaj¹ca/generuj¹ca
	virtual void Process(void);
	
};

#endif //CBLOCK_H
