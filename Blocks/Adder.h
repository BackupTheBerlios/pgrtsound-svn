#ifndef ADDER_H
#define ADDER_H

#include "CBlock.h"

class Adder : public CBlock{
public:

	Adder();
	virtual ~Adder();
    
    void Process(void);
};

#endif // ADDER_H
