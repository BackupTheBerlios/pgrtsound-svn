/* Generator sinus
 * Po³¹czenia
 *  wejœcie=3  (A, f, p)
 *  wyjœcie=1  (wy)
 * Algorytm
 *  wy = A*sin(2*pi*f*n+p)
 */


#ifndef GENERATOR_H
#define GENERATOR_H

#include "Module.h" // inheriting class's header file
 
class Generator : public Module
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

