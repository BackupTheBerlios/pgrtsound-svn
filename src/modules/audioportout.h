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
		static string GetTypeStatic();
		string GetType();
		
	private:
		Input inLeft;
		//input inRight;
};

#endif // AUDIOPORTOUT_H
