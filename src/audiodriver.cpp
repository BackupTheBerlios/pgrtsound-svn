#include "audiodriver.h"

AudioDriver::AudioDriver()
{
	TRACE("AudioDriver::AudioDriver()", "Inicjalizacja...");
	
	// raczej bezpieczne wartosci domyslne
	stream = NULL;
	sampleRate = 44100.0;
	sampleFormat = paFloat32;	// i basta - nie dajemy mozliwosci zmiany
	framesPerBuffer = 256;
	numBuffers = 0;				// automatyczny dobor ilosci buforow
	outputDevice = 0;			// domyslny w systemie
	numOutputChannels = 2;		// stereo
	inputDevice = -1;	// domyslnie nagrywanie wylaczone
	numInputChannels = 0;
	callbackFunction = NULL;
	callbackData = NULL;
	
	error = Pa_Initialize();
	if(error != paNoError)
		throw AudioDriverError( "AudioDriver::AudioDriver(): " + string(Pa_GetErrorText(error)) );
	else
		TRACE("AudioDriver::AudioDriver()", "Zainicjowany");
}

AudioDriver::~AudioDriver() {
	TRACE("AudioDriver::AudioDriver()", "Koncze...");

	if(Pa_StreamActive(stream)) {
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
		if(!Pa_StreamActive(stream)) {
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
		if(Pa_StreamActive(stream)) {
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
	int numDevices = Pa_CountDevices();
	const PaDeviceInfo* pdi;
	string inDevs, outDevs;
	int numInDevs = 0, numOutDevs = 0;
	char buff[20];
	
	cout << "AudioDriver: Dostepne urzadzenia..." << endl;
	for(int i = 0; i < numDevices; i++ ) {
		pdi = Pa_GetDeviceInfo(i);

		if(pdi->maxOutputChannels >= 2) {
            itoa(numOutDevs, buff, 10);
            outDevs += string("        (") + buff + ") " + string(pdi->name) + "\n";
            numOutDevs++;
		}
		
		if(pdi->maxInputChannels >= 2) {
            itoa(numInDevs, buff, 10);
            inDevs += string("        (") + buff + ") " + string(pdi->name) + "\n";
            numInDevs++;
		}
	}

	cout << "   Urzadzenia wejsciowe:" << endl << inDevs;
	cout << "   Urzadzenia wyjsciowe:" << endl << outDevs;
}

void AudioDriver::Open() {
	TRACE("AudioDriver::Open()",  "Otwieram strumien...");

	error = Pa_OpenStream(
			&stream,

			inputDevice,		/* input device */
			numInputChannels,	/* input channels */
			sampleFormat,		/* 32 bit floating point input */
			0,                  /* input driver info */

			outputDevice,		/* output device */
			numOutputChannels,	/* stereo output */
			sampleFormat,		/* 32 bit floating point output */
			0,

			sampleRate,         /* sample rate */
			framesPerBuffer,
			numBuffers,			/* number of buffers, if zero then use default minimum */
			paClipOff,			/* we won't output out of range samples so don't bother clipping them */
			callbackFunction,
			callbackData);

	if(error != paNoError)
		throw AudioDriverError( "AudioDriver::Open(): " + string(Pa_GetErrorText(error)) );
	else
		TRACE3("AudioDriver::Open()", "Otwarty strumien ", sampleRate, " Hz ");
}

void AudioDriver::EnableInput() {
	/* TODO (#1#): ZMIENIC!!! */
	inputDevice = outputDevice;
	numInputChannels = 2;
}

void AudioDriver::DisableInput() {
	/* TODO (#1#): ZMIENIC!!! */
	inputDevice = paNoDevice;
	numInputChannels = 0;
}
