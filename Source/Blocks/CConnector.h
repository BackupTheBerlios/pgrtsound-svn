#ifndef CCONNECTOR_H
#define CCONNECTOR_H

#include <string>

//! Kabelek do polaczen
/*!
Trala la la trala la la rrala la la mocium panie.
*/

using namespace std;


class CConnector{
public:

	CConnector(int bufferSize);
	virtual ~CConnector();
    
    int id;    //id polaczenia, unikalny
    string name;    //Unikalna w skali prokjektu (nazwa moze sie skladac z typ+id ale nie koniecznie
	
 
	// dostep do poczatka i konca 'kabla' - na razie jedno i to samo :o
	virtual float *In(void);
	virtual float *Out(void);
	
private:
	float *buffer;

};

#endif // CCONNECTOR_H
