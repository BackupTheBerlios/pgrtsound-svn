#include "callback.h"

//------------------------------------------------------------------------------
// callback dla portuadio
int paCallback( const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData )
{
	unsigned long i;

	Algorithm* alg = (Algorithm*)userData;

	float* put = alg->GetModule(0)->GetOutput(0)->GetSignal();	// AudioPortIn
	float* gen = alg->GetModule(1)->GetInput(0)->GetSignal();   // AudioPortOut
	float* in = (float*)inputBuffer;
	float* out = (float*)outputBuffer;

	// algorytm oblicza blok probek
	alg->Process();

//	if(in == NULL) {
//		for(i = 0; i < framesPerBuffer; i++) {
//			/* TODO (#1#): Zaimplementowac kanal lewy i prawy */
//			*out++ = *gen;
//			*out++ = *gen++;
//		}
//	}
//	else {
        for(i = 0; i < framesPerBuffer; i++) {
			/* TODO (#1#): Zaimplementowac kanal lewy i prawy */
			*put++ = ( (*in++) + (*in++) )*0.5f; // na razie miksowanie do mono
			*out++ = *gen;
			*out++ = *gen++;
		}
//	}

	return 0;
}
