#ifndef AUDIOPORTOUT_H
#define AUDIOPORTOUT_H

#include "module.h"

/*
 * Interfejs wyjscia systemu (algorytmu)
 */
class AudioPortOut : public Module {
	public:
		AudioPortOut();
		~AudioPortOut();
		REGISTER_MODULE( "AudioPortOut", AudioPortOut )
		
	private:
		Input inLeft;
		Input inRight;
};

#endif // AUDIOPORTOUT_H
