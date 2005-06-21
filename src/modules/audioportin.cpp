#include "audioportin.h"

AudioPortIn::AudioPortIn() : Module("audioportin", "AudioPortIn"),
	oLeft("left"), oRight("right")
{
	AddOutput(oLeft);
//	AddOutput(oRight);
}

AudioPortIn::~AudioPortIn() {
}
