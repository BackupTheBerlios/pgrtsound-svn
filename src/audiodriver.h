#ifndef AUDIODRIVER_H
#define AUDIODRIVER_H

#include <cstdlib>
#include <iostream>

#include "portaudio.h"

#define OUTPUT_DEVICE Pa_GetDefaultOutputDeviceID()

/**
 * No description
 */
class AudioDriver
{
	public:
		AudioDriver();
		~AudioDriver();
		void init(int samplingFreq, int numBits, int framesPerBuffer, int numBuffers);
		void start();
		void stop();
		void close();
		void setCallback(void* cbData);
		
	private:
		void* callbackData;
		int sampleRate;
		int numBits;
		int framesPerBuffer;
		int numBuffers;
		PaError error;
		PortAudioStream* stream;
};

#endif // AUDIODRIVER_H
