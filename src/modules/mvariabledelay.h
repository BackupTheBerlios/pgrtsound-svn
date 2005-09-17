#ifndef MVARIABLEDELAY_H
#define MVARIABLEDELAY_H

#include "module.h"

/**
 * No description
 */
class MVariableDelay : public Module
{
	public:
		MVariableDelay();
		~MVariableDelay();
		virtual void Process();
		virtual void Init();
		virtual void SampleRateChanged();
		REGISTER_MODULE( "Variable Delay", MVariableDelay )
		void ChangeMaxDelay();
		void SomethingChanged();
		
	protected:
		Input iIn;
		Input iDelayFraction;
		Output oOut;
		ParameterFloat pDelay;
		ParameterFloat pFeedback;
		unsigned long maxDelay, delay, readIndex, writeIndex;
		float* buffer, lastOutput, feedback;
};

#endif // MVARIABLEDELAY_H
