#ifndef CCONNECTOR_H
#define CCONNECTOR_H

//! Kabelek do polaczen
/*!
Trala la la trala la la rrala la la mocium panie.
*/
class CConnector{
public:

	CConnector(int bufferSize);
	virtual ~CConnector();
	
	// dostep do poczatka i konca 'kabla' - na razie jedno i to samo :o
	virtual float *In(void);
	virtual float *Out(void);
	
private:
	float *buffer;

};

#endif // CCONNECTOR_H
