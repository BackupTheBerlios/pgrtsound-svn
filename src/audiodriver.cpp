#include "audiodriver.h"

AudioDriver::AudioDriver() {
    TRACE( "AudioDriver::AudioDriver - Inicjalizacja...\n" );

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
        throw AudioDriverError( "AudioDriver::AudioDriver - " + string(Pa_GetErrorText(error)) );

    PaDeviceIndex devCount = Pa_GetDeviceCount();

    if(devCount <= 0) {
        TRACE( "AudioDriver::AudioDriver - Nie wykryto urzadzenia audio\n" );
    } else {
        // domyslny host i urzadzenia
        hostId = Pa_GetDefaultHostApi();
        outputDeviceId = Pa_GetDefaultOutputDevice();
        inputDeviceId = Pa_GetDefaultInputDevice();

        // raczej bezpieczne wartosci domyslne
        inputParameters.device = inputDeviceId;
        inputParameters.channelCount = 2;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency =
            Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = NULL;

        outputParameters.device = outputDeviceId;
        outputParameters.channelCount = 2;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency =
            Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        hostCount = -1;
        const PaHostApiInfo* apiInfo;
        // wykrywanie steorwnikow
        for(PaHostApiIndex hi = 0; hi < Pa_GetHostApiCount(); hi++) {
            apiInfo = Pa_GetHostApiInfo(hi);
            // wykrywanie urzadzen
            if(apiInfo->deviceCount > 0) {
                //PR(apiInfo->deviceCount);
                hostCount++;
                hosts.push_back(*apiInfo);
                // domyslny host?
                if(hostId == hi)
                    hostNum = hosts.size() - 1;
                // dodajmy to host API
                InputDevices in;
                perHostInDevs.push_back(in);
                OutputDevices out;
                perHostOutDevs.push_back(out);

                const PaDeviceInfo* paDevInfo;
                for(PaDeviceIndex di = 0; di < apiInfo->deviceCount; di++) {
                    int devIdx = Pa_HostApiDeviceIndexToDeviceIndex( hi, di);
                    paDevInfo = Pa_GetDeviceInfo(devIdx);
                    //PR(paDevInfo->name); PR(paDevInfo->hostApi);

                    // wykrycie urzadzen wejsciowych
                    if( paDevInfo->maxInputChannels >=2 ) {
                        DeviceInfo deviceInfo;
                        deviceInfo.deviceId = devIdx;
                        deviceInfo.hostApiId = paDevInfo->hostApi;
                        deviceInfo.name = paDevInfo->name;
                        perHostInDevs[hostCount].push_back(deviceInfo);
                        // czy urzadzenie o id = i jest domyslnym?
                        if( inputDeviceId == deviceInfo.deviceId )
                            inputDeviceNum = perHostInDevs[paDevInfo->hostApi].size() - 1;
                    }

                    // wykrycie urzadzen wyjsciowych
                    if( paDevInfo->maxOutputChannels >=2 ) {
                        DeviceInfo deviceInfo;
                        //deviceInfo.deviceId = Pa_HostApiDeviceIndexToDeviceIndex( hi, di);
                        deviceInfo.deviceId = devIdx;
                        deviceInfo.hostApiId = paDevInfo->hostApi;
                        deviceInfo.name = paDevInfo->name;
                        perHostOutDevs[hostCount].push_back(deviceInfo);
                        // czy urzadzenie o id = i jest domyslnym?
                        if( outputDeviceId == deviceInfo.deviceId )
                            outputDeviceNum = perHostOutDevs[paDevInfo->hostApi].size() - 1;
                    }
                }
            }
        }


        TRACE( "AudioDriver::AudioDriver - Zainicjowany\n" );
    }

    //	PR(hostId);
    //	PR(inputDeviceId);
    //	PR(outputDeviceId);
    //
    //	PR(hostNum);
    //	PR(inputDeviceNum);
    //	PR(outputDeviceNum);

}

AudioDriver::~AudioDriver() {
    TRACE("AudioDriver::~AudioDriver - Koncze...\n");

    Stop();
    Close();

    error = Pa_Terminate();
    if(error != paNoError)
        // desktruktor nie powinien rzucac wyjatkow!
        TRACE( "AudioDriver::~AudioDriver - %s\n", Pa_GetErrorText(error) );
    else
        TRACE( "AudioDriver::~AudioDriver - Zakonczony\n" );
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
                throw AudioDriverError( "AudioDriver::Start - " + string(Pa_GetErrorText(error)) );
            else
                TRACE("AudioDriver::Start - Strumien odtwarzany\n");
        }
    }
}

void AudioDriver::Stop() {
    if(stream != NULL) {
        if(Pa_IsStreamActive(stream)) {
            error = Pa_StopStream(stream);
            if(error != paNoError)
                throw AudioDriverError( "AudioDriver::Stop - " + string(Pa_GetErrorText(error)) );
            else
                TRACE("AudioDriver::Stop - Strumien zatrzymany\n");
        }
    }
}

void AudioDriver::Close() {
    if(stream != NULL) {
        error = Pa_CloseStream(stream);
        if(error != paNoError)
            //TRACE("AudioDriver::Close()",  Pa_GetErrorText(error));
            throw AudioDriverError( "AudioDriver::Close - " + string(Pa_GetErrorText(error)) );
        else {
            stream = NULL;
            TRACE("AudioDriver::Close - Strumien zakmniety\n");
        }
    }
}

void AudioDriver::PrintDevices() {
    TRACE("AudioDriver::PrintDevices - Lista wykrytych urzadzen dzwieku...\n");

    for(unsigned int h = 0; h < hosts.size(); h++) {
        TRACE( "Sterownik: %s\n", hosts[h].name );

        TRACE( "    Urzadzenia wejsciowe:\n" );
        for(unsigned int d = 0; d < perHostInDevs[h].size(); d++) {
            TRACE( "        [%i] %s (%s)\n", perHostInDevs[h][d].deviceId,
				perHostInDevs[h][d].name.c_str(), hosts[h].name );
        }

       TRACE("    Urzadzenia wyjsciowe:\n" );
        for(unsigned int d = 0; d < perHostOutDevs[h].size(); d++) {
            TRACE( "        [%i] %s (%s)\n", perHostOutDevs[h][d].deviceId,
				perHostOutDevs[h][d].name.c_str(), hosts[h].name );
        }
    }
}

void  AudioDriver::PrintConfiguration() {
    cout << "AudioDriver::PrintConfiguration - Biezaca konfiguracja sterownikia...\n" << endl;
    cout << "    Urzadzenie wejsciowe: " << inputParameters.device << endl;
    cout << "                 kanalow: " << inputParameters.channelCount << endl;
    cout << "              opoznienie: " << inputParameters.suggestedLatency << endl;
    cout << "    Urzadzenie wyjsciowe: " << outputParameters.device << endl;
    cout << "                 kanalow: " << outputParameters.channelCount << endl;
    cout << "              opoznienie: " << outputParameters.suggestedLatency << endl;
    cout << "      Czest. probkowania: " << sampleRate << endl;
    cout << "          Rozmiar bufora: " << framesPerBuffer << endl;
}

void AudioDriver::Open() {
    TRACE("AudioDriver::Open - Otwieram strumien...\n");
    Stop();
    Close();

    //PrintConfiguration();

    error = Pa_OpenStream(
                &stream,
                &inputParameters,
                //NULL,
                &outputParameters,
                sampleRate,
                framesPerBuffer,
                paNoFlag, /* we won't output out of range samples so don't bother clipping them */
                callbackFunction,
                callbackData);

    if(error != paNoError)
        throw AudioDriverError( "AudioDriver::Open - " + string(Pa_GetErrorText(error)) );
    else
        TRACE( "AudioDriver::Open - Otwarty strumien %.2f Hz, %d ramek/bufor\n",
			sampleRate, framesPerBuffer );
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
    } else
        return -1.0f;
    }

    PaTime AudioDriver::GetOutputLatency() const {
        if( stream != NULL) {
        const PaStreamInfo* si;
        si = Pa_GetStreamInfo(stream);
            return si->outputLatency;
        } else
            return -1.0f;
	}

        bool AudioDriver::IsConfigValid() {
        //TRACE2("AudioDriver::IsConfigValid()", "sampleRate = ", sampleRate);
        error = Pa_IsFormatSupported( &inputParameters, &outputParameters, sampleRate);
        if(error == paFormatIsSupported)
            return true;
        else
            return false;
    }

void AudioDriver::SetDevices(PaDeviceIndex inNum, PaDeviceIndex outNum) {
    inputDeviceNum = inNum;
    inputParameters.device = perHostInDevs[hostNum][inNum].deviceId;

    outputDeviceNum = outNum;
    outputParameters.device = perHostOutDevs[hostNum][outNum].deviceId;
}

string AudioDriver::GetLastErrorDescription() const {
    return Pa_GetErrorText(error);
}

void AudioDriver::SetSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
}

string AudioDriver::GetHostName(PaHostApiIndex num) {
    assert( (unsigned int)num <= hosts.size() - 1 );
    return hosts[num].name;
}

int AudioDriver::GetCurrentHostNum() {
    return hostNum;
}

void AudioDriver::SetHost(PaHostApiIndex hNum) {
    assert( (unsigned int)hNum <= hosts.size() - 1);
    hostNum = hNum;
}

double AudioDriver::GetSampleRate() const {
    return sampleRate;
}

void AudioDriver::SetBufferSize(unsigned long newBufferSize) {
    framesPerBuffer = newBufferSize;
}
