#ifndef MALLPASSFILTER_H
#define MALLPASSFILTER_H

#include "module.h"

/**
 No description
*/
class MAllpassFilter : public Module
{
	public:
		MAllpassFilter();
		~MAllpassFilter();
		virtual void Process();
	//	virtual void Init();
		REGISTER_MODULE( "Allpass Filter", MAllpassFilter )

	protected:
    	Input iIn;
    	Input iFreq;
		Output oOut;
		float* in, * out, freq, z, d, a, y;
};

#endif // MALLPASSFILTER_H
