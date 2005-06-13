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
		
	private:
		Input inLeft;
		//input inRight;
};

#endif // AUDIOPORTOUT_H
