#include "audioportout.h"

AudioPortOut::AudioPortOut() {
	type = "AudioPortOut";
	AddInput("audio in");
}

AudioPortOut::~AudioPortOut() {
}
