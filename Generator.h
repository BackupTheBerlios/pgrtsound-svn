
#ifndef GENERATOR_H
#define GENERATOR_H

#include "cblock.h" // inheriting class's header file

/**
 * Generator funkcji  A*sin(2*pi*f*n+p)
 * param[0] A
 * param[1] f
 * param[2] p
 */
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

