#include "noise.h"

int const q = 15;
float const c1 = (1 << q) - 1;
float const c2 = ((int)(c1 / 3)) + 1;
float const c3 = 1.f / c1;

Noise::Noise() : Module("noise", "New noise generator"), oOut("out") {
	AddOutput(&oOut);

	random = 0.f;
	noise = 0.f;

	/* Generate a new random seed from system time - do this once in your constructor */
	srand(time(0));
	
	PR(RAND_MAX);
}

Noise::~Noise() {
}

void Noise::Process() {
	float* out = oOut.GetSignal();

	for (int i = 0; i < Module::framesPerBlock; i++) {
		random = ((float)rand() / (float)(RAND_MAX + 1));
		noise = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
		*out++ = random;
	}
}
