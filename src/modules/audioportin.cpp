#include "audioportin.h"

AudioPortIn::AudioPortIn() : Module("AudioPortIn"),
	oLeft("audio input left"),
	oRight("audio input right")
{
	AddOutput( oLeft );
	AddOutput( oRight );
}

AudioPortIn::~AudioPortIn() {
}
