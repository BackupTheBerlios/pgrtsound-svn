/* Wzmacniacz
 * Po³¹czenia
 *  wejœcie=0 
 *  wyjœcie=1  (wy)
 *  parametr sta³y = 1 (fixedParam)
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
