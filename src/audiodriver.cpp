#include "audiodriver.h"

AudioDriver::AudioDriver()
{
	TRACE("AudioDriver::AudioDriver()", "Inicjalizacja...");

	error = Pa_Initialize();
	if(error != paNoError)
		throw AudioDriverError( "AudioDriver::AudioDriver(): " + string(Pa_GetErrorText(error)) );
	else
		TRACE("AudioDriver::AudioDriver()", "Zainicjowany");
	
	// raczej bezpieczne wartosci domyslne
	outputParameters.device = Pa_GetDefaultOutputDevice();	/* default output device */
	outputParameters.channelCount = 2;						/* stereo output */
	outputParameters.sampleFormat = paFloat32;				/* 32 bit floating point output */
	outputParameters.suggestedLatency =
		Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	inputParameters.device = paNoDevice;
	inputParameters.channelCount = 0;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = 0;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	stream = NULL;
	sampleRate = 44100.0;
	framesPerBuffer = 256;

	callbackFunction = NULL;
	callbackData = NULL;
}

AudioDriver::~AudioDriver() {
	TRACE("AudioDriver::AudioDriver()", "Koncze...");

	if(Pa_IsStreamActive(stream)) {
		Stop();
	}

	Close();
	
	error = Pa_Terminate();
	if(error != paNoError)
		TRACE( "AudioDriver::~AudioDriver()", Pa_GetErrorText(error) );
	else
		TRACE("AudioDriver::~AudioDriver()", "Zakonczony");
}

void  AudioDriver::SetCallback(PortAudioCallback* cbFunc, void* cbData) {
	callbackFunction = cbFunc;
	callbackData = cbData;
}

void AudioDriver::Open(double samplingFreq, unsigned long fpb, unsigned long numbuff) {
	sampleRate = samplingFreq;
	framesPerBuffer = fpb;
	numBuffers = numbuff;

	Open();
}

void AudioDriver::Start() {
    if(stream != NULL) {
		if(Pa_IsStreamStopped(stream)) {
			error = Pa_StartStream(stream);

			if(error != paNoError)
				throw AudioDriverError( "AudioDriver::Start(): " + string(Pa_GetErrorText(error)) );
			else
				TRACE("AudioDriver::Start()", "Strumien odtwarzany");
		}
	}
}

void AudioDriver::Stop() {
	if(stream != NULL) {
		if(Pa_IsStreamActive(stream)) {
			error = Pa_StopStream(stream);

			if(error != paNoError)
				throw AudioDriverError( "AudioDriver::Stop(): " + string(Pa_GetErrorText(error)) );
			else
				TRACE("AudioDriver::Stop()", "Strumien zatrzymany");
		}
	}
}

void AudioDriver::Close() {
	error = Pa_CloseStream(stream);
	if(error != paNoError)
		TRACE("AudioDriver::Close()",  Pa_GetErrorText(error));
	else
		TRACE("AudioDriver::Close()", "Strumien zakmniety");
}

void AudioDriver::PrintDevices() {
//	int numDevices = Pa_CountDevices();
//	const PaDeviceInfo* pdi;
//	string inDevs, outDevs;
//	int numInDevs = 0, numOutDevs = 0;
//	char buff[20];
//
//	cout << "AudioDriver: Dostepne urzadzenia..." << endl;
//	for(int i = 0; i < numDevices; i++ ) {
//		pdi = Pa_GetDeviceInfo(i);
//
//		if(pdi->maxOutputChannels >= 2) {
//            itoa(numOutDevs, buff, 10);
//            outDevs += string("        (") + buff + ") " + string(pdi->name) + "\n";
//            numOutDevs++;
//		}
//
//		if(pdi->maxInputChannels >= 2) {
//            itoa(numInDevs, buff, 10);
//            inDevs += string("        (") + buff + ") " + string(pdi->name) + "\n";
//            numInDevs++;
//		}
//	}
//
//	cout << "   Urzadzenia wejsciowe:" << endl << inDevs;
//	cout << "   Urzadzenia wyjsciowe:" << endl << outDevs;
}

void AudioDriver::Open() {
	TRACE("AudioDriver::Open()",  "Otwieram strumien...");

	error = Pa_OpenStream(
			&stream,

			&inputParameters,
			&outputParameters,

			sampleRate,         /* sample rate */
			framesPerBuffer,
			paClipOff,			/* we won't output out of range samples so don't bother clipping them */
			callbackFunction,
			callbackData);

	if(error != paNoError)
		throw AudioDriverError( "AudioDriver::Open(): " + string(Pa_GetErrorText(error)) );
	else
		TRACE3("AudioDriver::Open()", "Otwarty strumien ", sampleRate, " Hz ");
}

void AudioDriver::EnableInput() {
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = 2;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency =
		Pa_GetDeviceInfo( outputParameters.device )->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
}

void AudioDriver::DisableInput() {
	inputParameters.device = paNoDevice;
	inputParameters.channelCount = 0;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = 0;
	inputParameters.hostApiSpecificStreamInfo = NULL;
}

float AudioDriver::GetCPUUsage() const {
	if(stream != NULL)
		return Pa_GetStreamCpuLoad(stream);
	else
	    return 0.0;
}
