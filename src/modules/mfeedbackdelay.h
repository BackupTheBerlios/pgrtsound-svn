#ifndef MFEEDBACKDELAY_H
#define MFEEDBACKDELAY_H

#include "module.h"

/**
 No description
*/
class MFeedbackDelay : public Module
{
	public:
		MFeedbackDelay();
		~MFeedbackDelay();
		virtual void Process();
		virtual void Init();
		virtual void SampleRateChanged();
		REGISTER_MODULE( "Feedback Delay", MFeedbackDelay )
		void ChangeMaxDelay();
		void SomethingChanged();
		
	protected:
		Input iIn;
		Output oOut;
		ParameterFloat pDelay;
		ParameterFloat pFeedback;
		unsigned long maxDelay, delay, readIndex, writeIndex;
		float* buffer, feedback, lastOutput;
};

#endif // MFEEDBACKDELAY_H
