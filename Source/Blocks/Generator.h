/* Generator sinus
 * Po��czenia
 *  wej�cie=3  (A, f, p)
 *  wyj�cie=1  (wy)
 * Algorytm
 *  wy = A*sin(2*pi*f*n+p)
 */


#ifndef GENERATOR_H
#define GENERATOR_H

#include "CBlock.h" // inheriting class's header file
 
class Generator : public CBlock
{
	public:
		// class constructor
		Generator();
		// class destructor
		~Generator();
		// Process
		void Process(void);
		
	private:
        long n;
};

#endif //GENERATOR_H

