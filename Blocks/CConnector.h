#ifndef CCONNECTOR_H
#define CCONNECTOR_H

class CConnector{
public:

	CConnector(int bufferSize);
	~CConnector();
	
	// dostep do poczatka i konca 'kabla' - na razie jedno i to samo :o
	virtual float *In(void);
	virtual float *Out(void);
	
private:
	float *buffer;

};

#endif // CCONNECTOR_H
