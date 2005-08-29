#include "audioportin.h"

AudioPortIn::AudioPortIn() : Module("AudioPortIn"),
	oLeft("left"), oRight("right")
{
	AddOutput(oLeft);
//	AddOutput(oRight);
}

string AudioPortIn::GetTypeStatic() {
	return "audioportin";
}

string AudioPortIn::GetType() {
	return "audioportin";
}


AudioPortIn::~AudioPortIn() {
}
