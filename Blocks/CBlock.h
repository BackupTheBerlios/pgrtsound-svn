#include <string>
#include <vector>
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
	
	typedef std::vector <CConnector *> CConnectorVector;
	typedef CConnectorVector::iterator IterConnectorVector;
  
	//druty
	CConnectorVector inConnection;
    IterConnectorVector iterInConnection;
	int   inputCount;
	
	CConnectorVector outConnection;
    IterConnectorVector iterOutConnection;
	int   outputCount;
	
	//parametry sta�e
	float *param;
	int   paramCount;


    //class constructor
	CBlock();
	//class destructor
	virtual ~CBlock();
	
	
	//funkcjia obliczaj�ca/generuj�ca
	virtual void Process(void);
	
};

#endif //CBLOCK_H
