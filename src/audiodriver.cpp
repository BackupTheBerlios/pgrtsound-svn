#include "audiodriver.h"
#include "algorithm.h"

#define OUTPUT_DEVICE Pa_GetDefaultOutputDeviceID()

static int paCallback( void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer, PaTimestamp outTime, void *userData ) {
	unsigned long i;

	Algorithm* alg = (Algorithm*)userData;
		
	float* gen = alg->process();
	float* out = (float*)outputBuffer;

	for(i = 0; i < framesPerBuffer; i++) {
		//cout << *gen << endl;
		*out++ = *gen;
		*out++ = *gen++;
	}
	return 0;
}

AudioDriver::AudioDriver()
{
	sampleRate = 44100;
	numBits = 16;
	framesPerBuffer = 64;
	numBuffers = 0;

	error = Pa_Initialize();
}

AudioDriver::~AudioDriver()
{
	Pa_CloseStream(stream);
	Pa_Terminate();
}

void AudioDriver::setCallback(void* cbData) {
	callbackData = cbData;
}

void AudioDriver::init(int sFreq, int nBits, int fPerBuffer, int nBuffers) {
	sampleRate = sFreq;
	numBits = nBits;
	framesPerBuffer = fPerBuffer;
	numBuffers = nBuffers;
	 
	error = Pa_OpenStream(
			&stream,
			paNoDevice,			/* default input device */
			0,					/* no input */
			paFloat32,			/* 32 bit floating point input */
			0,
			OUTPUT_DEVICE,
			2,					/* stereo output */
			paFloat32,			/* 32 bit floating point output */
			0,
			sampleRate,
			framesPerBuffer,
			numBuffers,			/* number of buffers, if zero then use default minimum */
			paClipOff,			/* we won't output out of range samples so don't bother clipping them */
			paCallback,
			callbackData);

	#ifndef NDEBUG
	printf("PortAudio: SR = %d, BufSize = %d, devID = %d\n",
		sampleRate, framesPerBuffer, OUTPUT_DEVICE);
   	#endif
}

void AudioDriver::start() {
	Pa_StartStream(stream);
}

void AudioDriver::stop() {
	Pa_StopStream(stream);
}

