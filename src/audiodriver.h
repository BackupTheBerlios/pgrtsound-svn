#ifndef AUDIODRIVER_H
#define AUDIODRIVER_H

#include <cstdlib>
#include <iostream>
#include <string>

#include "debug.h"
#include "portaudio.h"

using namespace std;

class AudioDriverError : public runtime_error {
    public:
        AudioDriverError (const string& msg = "") : runtime_error(msg) {}
};

/**
 * PortAudio wrapper
 */
class AudioDriver
{
	public:
		AudioDriver();
		~AudioDriver();
		void Open(double samplingFreq, unsigned long framesPerBuffer, unsigned long numBuffers);
		void Start();
		void Stop();
		void Close();
		void SetCallback(PortAudioCallback* cbFunc, void* cbData);
		void EnableInput();
		void DisableInput();
		void PrintDevices();

	private:
		void Open();

		double				sampleRate;
		unsigned long		framesPerBuffer, numBuffers;
		PaDeviceID 			outputDevice, inputDevice;
		int                 numOutputChannels, numInputChannels;
		PaSampleFormat      sampleFormat;
		void*				callbackData;
		PortAudioCallback*	callbackFunction;
		PortAudioStream*	stream;
		PaError				error;
};

#endif // AUDIODRIVER_H
