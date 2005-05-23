#ifndef AUDIOPORTIN_H
#define AUDIOPORTIN_H

#include "module.h"

/**
 * No description
 */
class AudioPortIn : public Module {
	public:
		AudioPortIn();
		~AudioPortIn();

	private:
		Output oLeft;
		Output oRight;
};

#endif // AUDIOPORTIN_H
