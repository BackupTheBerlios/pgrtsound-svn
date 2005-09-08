#ifndef MADSRLINEAR_H
#define MADSRLINEAR_H

#include "module.h"

/*
 Generator obwiedni ADSR.
*/
class MADSRLinear : public Module {
	public:
		MADSRLinear();
		~MADSRLinear();
		virtual void Process();
		virtual void Init();
		REGISTER_MODULE( "ADSR Envelope", MADSRLinear )
		void Update();
		//void UpdateAttack();
		//void UpdateDecay();
		void TriggerEnvelope();
		void ReleaseEnvelope();

	protected:
   		Input iGateIn;
		Output oEnvelope;
		ParameterFloat pAttackTime, pDecayTime, pSustainLevel, pReleaseTime;
		unsigned int currentTime, attackTime, decayTime, releaseTime;
		float currentLevel;
		bool gateOn;
		enum { OFF, ATTACK, DECAY, SUSTAIN, RELEASE };
		int state;
		float attackCoeff, decayCoeff, releaseCoeff;
		float sustainLevel;
};

#endif // MADSRLINEAR_H
