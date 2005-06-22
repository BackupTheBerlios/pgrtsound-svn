#ifndef NOISE_H
#define NOISE_H

#include <cstdlib>
#include <ctime>

#include "../module.h" // inheriting class's header file

/**
 * Generator szumu.
 */
class Noise : public Module
{
	public:
		Noise();
		~Noise();
		void Process();
static Module* Create();
	private:
		Output oOut;

		/* Setup constants */
		const static int q;
		const static float c1;
		const static float c2;
		const static float c3;

		/* random number in range 0 - 1 not including 1 */
		float random;
		/* the white noise */
		float noise;
};

#endif // NOISE_H
