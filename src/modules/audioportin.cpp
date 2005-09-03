#include "audioportin.h"

AudioPortIn::AudioPortIn() : Module("AudioPortIn"),
	oLeft("left"), oRight("right")
{
	AddOutput(oLeft);
//	AddOutput(oRight);
}

AudioPortIn::~AudioPortIn() {
}
