/* Sumator
 * Po��czenia
 *  wej�cie=2  (we1 i we2)
 *  wyj�cie=1  (wy)
 * Algorytm
 *  wy = we1 + we2
 */


#ifndef ADDER_H
#define ADDER_H

#include "Module.h"

class Adder : public Module{
public:

	Adder();
	virtual ~Adder();
    
    void Process(void);
};

#endif // ADDER_H
