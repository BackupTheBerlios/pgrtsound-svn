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

typedef int (PortAudioCallback)( const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData );

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
		float GetCPUUsage() const;

	private:
		void Open();

		double				sampleRate;
		unsigned long		framesPerBuffer, numBuffers;
		PaDeviceIndex		outputDevice, inputDevice;
		int                 numOutputChannels, numInputChannels;
		PaSampleFormat      sampleFormat;
		void*				callbackData;
		PortAudioCallback*	callbackFunction;
		PaStream*	stream;
		PaStreamParameters	inputParameters, outputParameters;
		PaError				error;
};

#endif // AUDIODRIVER_H
