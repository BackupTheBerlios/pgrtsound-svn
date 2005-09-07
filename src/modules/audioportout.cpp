#include "audioportout.h"

AudioPortOut::AudioPortOut() : Module("AudioPortOut"),
	inLeft("audio output left"),
	inRight("audio output right")
	
{
	AddInput(inLeft);
	AddInput(inRight);
}

AudioPortOut::~AudioPortOut() {
}
