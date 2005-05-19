#include "audioportout.h"

AudioPortOut::AudioPortOut() : Module("audioportout", "AudioPortOut"),
	inLeft("wejscie L")
{
	AddInput(&inLeft);
}

AudioPortOut::~AudioPortOut() {
}
