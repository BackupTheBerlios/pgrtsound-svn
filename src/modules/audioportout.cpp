#include "audioportout.h"

AudioPortOut::AudioPortOut() : Module("AudioPortOut"),
	inLeft("wejscie L")
{
	AddInput(inLeft);
}

AudioPortOut::~AudioPortOut() {
}
