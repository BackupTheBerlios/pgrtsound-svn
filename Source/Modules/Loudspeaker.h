#ifndef LOUDSPEAKER_H
#define LOUDSPEAKER_H

#include "CBlock.h"

#if defined(__WINDOWS_DS__)
#include "..\RTAudio\RTAudio.h"
#endif


class Loudspeaker :  CBlock{
public:

	Loudspeaker();
	virtual ~Loudspeaker();
};

#endif // LOUDSPEAKER_H
