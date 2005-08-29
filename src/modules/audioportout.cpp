#include "audioportout.h"

AudioPortOut::AudioPortOut() : Module("AudioPortOut"),
	inLeft("wejscie L")
{
	AddInput(inLeft);
}

string AudioPortOut::GetTypeStatic() {
	return "audioportout";
}

string AudioPortOut::GetType() {
	return "audioportout";
}

AudioPortOut::~AudioPortOut() {
}
