#include "callback.h"

//------------------------------------------------------------------------------
// callback dla portuadio
int paCallback( const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData )
{
	unsigned long i;

	/* Prevent unused variable warnings. */
	(void) timeInfo;
	(void) statusFlags;

	Algorithm* alg = (Algorithm*)userData;

	float* recL = alg->GetInputPort()->GetOutput(0)->GetSignal(); // AudioPortIn
	float* recR = alg->GetInputPort()->GetOutput(1)->GetSignal();
	
	float* playL = alg->GetOutputPort()->GetInput(0)->GetSignal(); // AudioPortOut
	float* playR = alg->GetOutputPort()->GetInput(1)->GetSignal();

	float* in = (float*)inputBuffer;
	float* out = (float*)outputBuffer;

	// algorytm oblicza blok probek
	alg->Process();

	for( i = 0; i < framesPerBuffer; i++ ) {
		*recL++ = *in++;
		*recR++ = *in++;
		
		*out++ = *playL++;
		*out++ = *playR++;
	}

	return 0;
}
