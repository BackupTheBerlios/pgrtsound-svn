#include "audioportout.h"

AudioPortOut::AudioPortOut() {
	type = "AudioPortOut";
	addInput("audio in");
}

AudioPortOut::~AudioPortOut() {
}
