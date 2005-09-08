#include "madsrlinear.h"

MADSRLinear::MADSRLinear() :
	Module( "New ADSR" ),
	iGateIn( "gate input" ),
	oEnvelope( "envelope output" ),
	pAttackTime( "Attack time" ),
	pDecayTime( "Decay time" ),
	pSustainLevel( "Sustain level" ),
	pReleaseTime( "Release time" )
{
	AddInput( iGateIn );
	AddOutput( oEnvelope );
	
	AddParameter( pAttackTime );
	AddParameter( pDecayTime );
	AddParameter( pSustainLevel );
	AddParameter( pReleaseTime );
	
	//pAttackTime.Bound( 0, 5000, 1 ); // milisekund
	//pDecayTime.Bound( 0, 5000, 1 );
	//pSustainLevel.Bound( 0, 1, 0.01 );
	//pReleaseTime.Bound( 0, 5000, 1 );
	
	pAttackTime.SetValue( 100 );
	pDecayTime.SetValue( 100 );
	pSustainLevel.SetValue( 0.5 );
	pReleaseTime.SetValue( 500 );

	currentLevel = 0;
	state = OFF;
	gateOn = false;
}

MADSRLinear::~MADSRLinear() {
}

void MADSRLinear::Init() {
}

void MADSRLinear::Update() {
	attackTime = (unsigned int) ( pAttackTime.GetValue() * Module::sampleRate * 0.001f );
	decayTime = (unsigned int) ( pDecayTime.GetValue() * Module::sampleRate * 0.001f );
	releaseTime = (unsigned int) ( pReleaseTime.GetValue() * Module::sampleRate * 0.001f );
	sustainLevel = pSustainLevel.GetValue();

	//cout << "attackTime = " << attackTime << endl;
	//cout << "decayTime = " << decayTime << endl;
	//cout << "releaseTime = " << releaseTime << endl;
	//cout << "sustainLevel = " << sustainLevel << endl;

   	attackCoeff = ( 1.0f - currentLevel ) / attackTime;
   	decayCoeff = ( sustainLevel - 1.0f ) / decayTime;
	releaseCoeff = -sustainLevel / releaseTime;
}

void MADSRLinear::TriggerEnvelope() {
	currentTime = 0;
	Update();
	state = ATTACK;
	//cout << "ADSR - trig" << endl;
}

void MADSRLinear::ReleaseEnvelope() {
	if( state == SUSTAIN ) {
		state = RELEASE;
		currentTime = 0;
	}

	//cout << "ADSR - release" << endl;
}

void MADSRLinear::Process() {
	float* in = iGateIn.GetSignal();
	float* out = oEnvelope.GetSignal();

	// TODO: z czest bloku tylko ???
	if( !gateOn ) {
		if( *in > 0.5 ) {
			TriggerEnvelope();
			gateOn = true;
   		}
	}
	else {
		if( *in < 0.5 ) {
			ReleaseEnvelope();
			gateOn = false;
   		}
	}

	for( unsigned long i = 0; i < Module::framesPerBlock; i++ ) {
		switch ( state ) {
			case ATTACK:
				if( currentTime < attackTime ) {
					currentLevel += attackCoeff;
					*out++ = currentLevel;
					currentTime++;
					break;
				}
				state = DECAY;
				currentTime = 0;

			case DECAY:
   				if( currentTime < decayTime ) {
					currentLevel += decayCoeff;
					*out++ = currentLevel;
					currentTime++;
					break;
				}

				currentTime = 0;
				if( gateOn ) {
					state = SUSTAIN;
				}
				else state = RELEASE;

			case SUSTAIN:
				*out++ = sustainLevel;
				break;
				
			case RELEASE:
				if( currentTime < releaseTime ) {
					currentLevel += releaseCoeff;
					*out++ = currentLevel;
					currentTime++;
					break;
				}

				currentTime = 0;
				state = OFF;

			case OFF:
				*out++ = 0.0f;
				break;
		}
	}
}
