/* Wzmacniacz
 * Po³¹czenia
 *  wejœcie=2  (we, wzm)
 *  wyjœcie=1  (wy)
 * Algorytm
 *  wy = we * wzm
 */

#ifndef AMPLIFIER_H
#define AMPLIFIER_H


#include "CBlock.h" // inheriting class's header file

/**
* Powiedzmy, ze ma to byc wzmacniacz
* Wzmacniacz wbrew nazwie moze czasami tlumic :P
*/
class Amplifier : public CBlock
{
	public:
		// class constructor
		Amplifier();
		// class destructor
		~Amplifier();
		
		//! Funckcja przetwarzajaca
		/*!
			A more elaborate description of funckja na la la la.
		*/
		void Process(void);
};

#endif //AMPLIFIER_H

