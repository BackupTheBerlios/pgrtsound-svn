/* Wzmacniacz
 * Po��czenia
 *  wej�cie=0 
 *  wyj�cie=1  (wy)
 *  parametr sta�y = 1 (fixedParam)
 * Algorytm
 *  wy = fixedParam
 */

#ifndef FIXEDPARAMETER_H
#define FIXEDPARAMETER_H

#include "CBlock.h" 

class FixedParameter : public CBlock{
public:

    void Process(void);

	FixedParameter();
	virtual ~FixedParameter();
};

#endif // FIXEDPARAMETER_H
