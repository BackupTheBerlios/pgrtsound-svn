#ifndef AUDIODRIVER_H
#define AUDIODRIVER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

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

struct DeviceInfo {
    PaDeviceIndex id;
    PaHostApiIndex hostApiId;
	string name;
};

struct HostInfo {
    PaHostApiIndex id;
	string name;
};

/**
 * PortAudio wrapper
 */
class AudioDriver
{
	public:
		AudioDriver();
		~AudioDriver();
		void Open();
		void Start();
		void Stop();
		void Close();
		void SetCallback(PortAudioCallback* cbFunc, void* cbData);
		void PrintDevices();
		void PrintConfiguration();
		bool IsConfigValid();
		void SetSampleRate(double newSampleRate);
		void SetHost(PaHostApiIndex hNum);
		void SetDevices(PaDeviceIndex inDevNum, PaDeviceIndex outDevNum);
		float GetCPUUsage() const;
		int GetInputDeviceCount() const;
		int GetOutputDeviceCount() const;
		int GetHostCount() const;
		string GetHostName(PaHostApiIndex id);
		string GetInputDeviceName(PaDeviceIndex id);
		string GetOutputDeviceName(PaDeviceIndex id);
		int GetCurrentHostNum();
		int GetCurrentInputDeviceNum();
		int GetCurrentOutputDeviceNum();
		PaTime GetInputLatency() const;
		PaTime GetOutputLatency() const;
		string GetLastErrorDescription() const;

	private:
		double				sampleRate;
		unsigned long		framesPerBuffer, numBuffers;
		PaDeviceIndex		outputDeviceId, inputDeviceId;
		PaHostApiIndex      hostId;
		int                 outputDeviceNum, inputDeviceNum;
		int                 numOutputChannels, numInputChannels;
		int                 hostNum;
		PaSampleFormat      sampleFormat;
		void*				callbackData;
		PortAudioCallback*	callbackFunction;
		PaStream*			stream;
		PaStreamParameters	inputParameters, outputParameters;
		PaError				error;
		vector<HostInfo>    hosts;
		
		typedef vector<DeviceInfo> InputDevices;
		typedef vector<DeviceInfo> OutputDevices;
		
		vector< InputDevices > perHostInDevs;
		vector< OutputDevices > perHostOutDevs;
};

#endif // AUDIODRIVER_H
