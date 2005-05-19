#include "audioportout.h"

AudioPortOut::AudioPortOut() :
	Module("audioportput", "Audio Port Out"),
	inLeft("wejscie L")
{
	//type = "AudioPortOut";

	//inLeft.SetName("wejscie L");
	AddInput(&inLeft);
}

AudioPortOut::~AudioPortOut() {
}
