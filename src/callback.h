#ifndef CALLBACK_H
#define CALLBACK_H

#include "algorithm.h"
#include "portaudio.h"

extern int paCallback( void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, PaTimestamp outTime, void *userData );

#endif // CALLBACK_H
