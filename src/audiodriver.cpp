#include "audiodriver.h"

AudioDriver::AudioDriver() {
	TRACE("AudioDriver::AudioDriver()", "Inicjalizacja...");

	stream = NULL;
	sampleRate = 44100.0;
	framesPerBuffer = 256;
	callbackFunction = NULL;
	callbackData = NULL;

	// domyslnie brak urzadzen
	outputParameters.device = paNoDevice;
	outputParameters.channelCount = 0;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = 0;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	inputParameters.device = paNoDevice;
	inputParameters.channelCount = 0;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency = 0;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	error = Pa_Initialize();
	if(error != paNoError)
		throw AudioDriverError( "AudioDriver::AudioDriver(): " + string(Pa_GetErrorText(error)) );

	PaDeviceIndex devCount = Pa_GetDeviceCount();

	if(devCount <= 0) {
		TRACE("AudioDriver::AudioDriver()",  "Nie wykryto urzadzenia audio");
	} else {
       	// jest jakas karta dzwiekowa
       	
		// domyslny host i urzadzenia
		hostId = Pa_GetDefaultHostApi();
		outputDeviceId = Pa_GetDefaultOutputDevice();
		inputDeviceId = Pa_GetDefaultInputDevice();

		// raczej bezpieczne wartosci domyslne
		outputParameters.device = outputDeviceId;
		outputParameters.channelCount = 2;				/* stereo output */
		outputParameters.sampleFormat = paFloat32;		/* 32 bit floating point output */
		outputParameters.suggestedLatency =
			Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
		outputParameters.hostApiSpecificStreamInfo = NULL;

		inputParameters.device = inputDeviceId;
		inputParameters.channelCount = 2;
		inputParameters.sampleFormat = paFloat32;
		inputParameters.suggestedLatency =
			Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
		inputParameters.hostApiSpecificStreamInfo = NULL;

		cout << "wykrycie sterow" << endl;
		// wykrywanie steorwnikow
	    PaHostApiIndex hostCount = 0;
	    const PaHostApiInfo* apiInfo;
		hostCount = Pa_GetHostApiCount();
		PR(hostCount);
		for(PaHostApiIndex i = 0; i < hostCount-1; i++) {
			HostInfo hi;
			apiInfo = Pa_GetHostApiInfo(i);
			if(apiInfo->deviceCount > 0) {
				hi.id = i;
				hi.name = apiInfo->name;
				hosts.push_back(hi);
				InputDevices in; perHostInDevs.push_back(in);
				OutputDevices out; perHostOutDevs.push_back(out);
				// domyslny host?
				if(hostId == i)
				    hostNum = hosts.size() - 1;
			}
		}

		cout << "wykrycie wejsc" << endl;
		// wykrycie urzadzen wejsciowych

		const PaDeviceInfo* devInfo;
		for(PaDeviceIndex i = 0; i < devCount; i++) {
			devInfo = Pa_GetDeviceInfo(i);
			if( devInfo->maxInputChannels >=2 ) {
				DeviceInfo di;
				di.id = i;
				di.hostApiId = devInfo->hostApi;
				di.name = devInfo->name;
				perHostInDevs[devInfo->hostApi].push_back(di);
				// czy urzadzenie o id = i jest domyslnym?
				if( (inputDeviceId == i) and (hostId == devInfo->hostApi) )
				    inputDeviceNum = perHostInDevs[devInfo->hostApi].size() - 1;
			}
	  	}

		cout << "wykrycie wyjsc" << endl;
	   // wykrycie urzadzen wyjsciowych
	  	for(PaDeviceIndex i = 0; i < devCount; i++) {
			devInfo = Pa_GetDeviceInfo(i);
			if( devInfo->maxOutputChannels >=2 ) {
				DeviceInfo di;
				di.id = i;
				di.hostApiId = devInfo->hostApi;
				di.name = devInfo->name;
				perHostOutDevs[devInfo->hostApi].push_back(di);
				//inputDevices.push_back(di);
				// czy urzadzenie o id = i jest domyslnym?
				if( (outputDeviceId == i) and (hostId == devInfo->hostApi) )
				    outputDeviceNum = perHostOutDevs[devInfo->hostApi].size() - 1;
			}
	  	}

		TRACE("AudioDriver::AudioDriver()", "Zainicjowany");
	}
}

AudioDriver::~AudioDriver() {
	TRACE("AudioDriver::AudioDriver()", "Koncze...");

	if(Pa_IsStreamActive(stream)) {
		Stop();
	}

	Close();
	
	error = Pa_Terminate();
	if(error != paNoError)
	    // desktruktor nie powinien rzucac wyjatkow!
		TRACE( "AudioDriver::~AudioDriver()", Pa_GetErrorText(error) );
	else
		TRACE("AudioDriver::~AudioDriver()", "Zakonczony");
}

void  AudioDriver::SetCallback(PortAudioCallback* cbFunc, void* cbData) {
	callbackFunction = cbFunc;
	callbackData = cbData;
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
    if(stream != NULL) {
		error = Pa_CloseStream(stream);
		if(error != paNoError)
			//TRACE("AudioDriver::Close()",  Pa_GetErrorText(error));
			throw AudioDriverError( "AudioDriver::Close(): " + string(Pa_GetErrorText(error)) );
		else {
			stream = NULL;
			TRACE("AudioDriver::Close()", "Strumien zakmniety");
		}
	}
}

void AudioDriver::PrintDevices() {
	TRACE("AudioDriver::PrintDevices()", "Lista wykrytych urzadzen dzwieku...");

	for(int h = 0; h < hosts.size(); h++) {
		cout << "Sterownik: " << hosts[h].name << endl;
		
		cout << "    Uradzenia wejsciowe:" << endl;
		for(int d = 0; d < perHostInDevs[h].size(); d++) {
            cout << "        " << "[" << perHostInDevs[h][d].id << "] "
				<< perHostInDevs[h][d].name << " ("
				<< hosts[perHostInDevs[h][d].hostApiId].name << ")" << endl;
		}
		
		cout << "    Uradzenia wyjsciowe:" << endl;
		for(int d = 0; d < perHostOutDevs[h].size(); d++) {
            cout << "        " << "[" << perHostOutDevs[h][d].id << "] "
				<< perHostOutDevs[h][d].name << " ("
				<< hosts[perHostOutDevs[h][d].hostApiId].name << ")" << endl;
		}
	}
}

void  AudioDriver::PrintConfiguration() {
//	TRACE("AudioDriver::PrintConfiguration()", "Biezaca konfiguracja sterownikia...");
//	cout << "    Urzadzenie wejsciowe: " << perHostInDevs[hostNum][inputDeviceNum].name
//		<< " ("	<< hosts[ perHostInDevs[hostNum][inputDeviceNum].hostApiId ].name << ")" << endl;
//	cout << "    Urzadzenie wyjsciowe: " << perHostOutDevs[hostNum][outputDeviceNum].name
//		<< " ("	<< hosts[ perHostOutDevs[hostNum][outputDeviceNum].hostApiId ].name << ")" << endl;
//	cout << "    Czest. probkowania: " << sampleRate << endl;
}

void AudioDriver::Open() {
	TRACE("AudioDriver::Open()",  "Otwieram strumien...");

	Stop();
	Close();

	PrintConfiguration();

	error = Pa_OpenStream(
			&stream,
			&inputParameters,
			&outputParameters,
			sampleRate,
			framesPerBuffer,
			paClipOff,			/* we won't output out of range samples so don't bother clipping them */
			callbackFunction,
			callbackData);

	if(error != paNoError)
		throw AudioDriverError( "AudioDriver::Open(): " + string(Pa_GetErrorText(error)) );
	else
		TRACE3("AudioDriver::Open()", "Otwarty strumien ", sampleRate, " Hz ");
}

float AudioDriver::GetCPUUsage() const {
	if(stream != NULL)
		return Pa_GetStreamCpuLoad(stream);
	else
	    return 0.0;
}

int AudioDriver::GetHostCount() const {
	return hosts.size();
}

int AudioDriver::GetInputDeviceCount() const {
	return perHostInDevs[hostNum].size();
}

int AudioDriver::GetOutputDeviceCount() const {
	return perHostOutDevs[hostNum].size();
}

string AudioDriver::GetInputDeviceName(int id) {
	return perHostInDevs[hostNum][id].name;
}

string AudioDriver::GetOutputDeviceName(int id) {
	return perHostOutDevs[hostNum][id].name;
}

int AudioDriver::GetCurrentInputDeviceNum() {
	return inputDeviceNum;
}

int AudioDriver::GetCurrentOutputDeviceNum() {
	return outputDeviceNum;
}

PaTime AudioDriver::GetInputLatency() const {
	if( stream != NULL) {
       	const PaStreamInfo* si;
        si = Pa_GetStreamInfo(stream);
        return si->inputLatency;
	}
	else return -1.0f;
}

PaTime AudioDriver::GetOutputLatency() const {
	if( stream != NULL) {
       	const PaStreamInfo* si;
        si = Pa_GetStreamInfo(stream);
        return si->outputLatency;
	}
	else return -1.0f;
}

bool AudioDriver::IsConfigValid() {
	//TRACE2("AudioDriver::IsConfigValid()", "sampleRate = ", sampleRate);
	error = Pa_IsFormatSupported( &inputParameters, &outputParameters, sampleRate);
	if(error == paFormatIsSupported)
	    return true;
    else return false;
}

void AudioDriver::SetDevices(PaDeviceIndex inNum, PaDeviceIndex outNum) {
	inputDeviceNum = inNum;
	inputParameters.device = perHostInDevs[hostNum][inNum].id;
	
	outputDeviceNum = outNum;
	outputParameters.device = perHostOutDevs[hostNum][outNum].id;
}

string AudioDriver::GetLastErrorDescription() const {
	return Pa_GetErrorText(error);
}

void AudioDriver::SetSampleRate(double newSampleRate) {
	sampleRate = newSampleRate;
}

string AudioDriver::GetHostName(PaHostApiIndex num) {
	assert(num <= hosts.size() - 1);
	return hosts[num].name;
}

int AudioDriver::GetCurrentHostNum() {
	return hostNum;
}

void AudioDriver::SetHost(PaHostApiIndex hNum) {
	assert(hNum <= hosts.size() - 1);
	hostNum = hNum;
}

