#include "audioportout.h"

AudioPortOut::AudioPortOut() {
	type = "AudioPortOut";

	inLeft.SetName("wejscie L");
	AddInput(&inLeft);
}

AudioPortOut::~AudioPortOut() {
}
