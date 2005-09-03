#ifndef AUDIOPORTIN_H
#define AUDIOPORTIN_H

#include "module.h"

/**
 * Modul wejscia audio systemu (algorytmu)
 */
class AudioPortIn : public Module {
	public:
		AudioPortIn();
		~AudioPortIn();
		REGISTER_MODULE( "AudioPortIn", AudioPortIn )

	private:
		Output oLeft;
		Output oRight;
};

#endif // AUDIOPORTIN_H
