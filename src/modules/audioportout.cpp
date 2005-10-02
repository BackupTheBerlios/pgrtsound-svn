#include "audioportout.h"

AudioPortOut::AudioPortOut() : 
	inLeft("audio output left"),
	inRight("audio output right")
	
{
    SetName("AudioPortOut");
	AddInput(inLeft);
	AddInput(inRight);
}

AudioPortOut::~AudioPortOut() {
}
