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
		static string GetTypeStatic();
		string GetType();

	private:
		Output oLeft;
		Output oRight;
};

#endif // AUDIOPORTIN_H
