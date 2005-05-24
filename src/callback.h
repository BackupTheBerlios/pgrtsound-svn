#ifndef CALLBACK_H
#define CALLBACK_H

#include "algorithm.h"
#include "portaudio.h"

extern int paCallback( const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData );

#endif // CALLBACK_H
