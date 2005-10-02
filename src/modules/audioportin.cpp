#include "audioportin.h"

AudioPortIn::AudioPortIn() :
	oLeft("audio input left"),
	oRight("audio input right")
{
    SetName("AudioPortIn");
	AddOutput( oLeft );
	AddOutput( oRight );
}

AudioPortIn::~AudioPortIn() {
}
