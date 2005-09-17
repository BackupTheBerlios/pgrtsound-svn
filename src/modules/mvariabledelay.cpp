#include "mvariabledelay.h"

MVariableDelay::MVariableDelay() :
	Module( "Feedback Delay" ),
	iIn("Wejœcie audio"),
	iDelayFraction( "U³amek opóŸnienia [0 - 1]" ),
	oOut("Wyjœcie audio"),
	pDelay( "OpóŸnienie [ms]" ),
	pFeedback("Sprzê¿enie zwrotne")
{
	AddInput( iIn );
	AddInput( iDelayFraction );
	AddOutput( oOut );
	AddParameter( pDelay );
	AddParameter( pFeedback );
	
	pDelay.Bound( 0, (unsigned long)Module::sampleRate, 1 );
	pDelay.SetValue( 500.0f );
	
	pFeedback.SetValue( 0.0f );
	
	buffer = NULL;
	maxDelay = 0;
	lastOutput = 0;
}

MVariableDelay::~MVariableDelay() {
}

void MVariableDelay::Init() {
	ChangeMaxDelay();
}

void MVariableDelay::SampleRateChanged() {
   	pDelay.Bound( 0, (unsigned long)Module::sampleRate, 1 );
    ChangeMaxDelay();
}

void MVariableDelay::ChangeMaxDelay() {
	delete []buffer;
	maxDelay = (unsigned long)Module::sampleRate; // maksymalne opoznienie
	buffer = new float[ maxDelay ];
	
	// zerowanie bufora
	for( unsigned long i = 0; i < maxDelay; i++ )
		buffer[i] = 0.0f;

	writeIndex = 0;
	
	SomethingChanged();
}

void MVariableDelay::SomethingChanged() {
	delay = (unsigned long)( pDelay.GetValue() * Module::sampleRate * 0.001f );
	delay = ( delay > maxDelay )? maxDelay : delay;

	feedback = pFeedback.GetValue();
	feedback = ( feedback > 0.99f ) ? 0.99f : feedback;

	readIndex = writeIndex + (maxDelay - delay);
	readIndex = readIndex % maxDelay;

	TRACE( "MVariableDelay::SomethingChanged - delay = %i\n", delay );
}

void MVariableDelay::Process() {
	float* in = iIn.GetSignal();
	float* delayFrac = iDelayFraction.GetSignal();
	float* out = oOut.GetSignal();

   	for( unsigned long i = 0; i < Module::framesPerBlock; i++ ) {
		buffer[ writeIndex ] = *in++ + lastOutput * feedback;
		
		float currentDelay = (float)delay * (*delayFrac++);
		float readIdxFloat = (float)writeIndex + ( (float)maxDelay -
			currentDelay );
		float fract = readIdxFloat - floorf(readIdxFloat);
		unsigned long readIdx = (unsigned long)( floorf(readIdxFloat) );
		unsigned long readIdx2 = readIdx + 1;
		readIdx = readIdx % maxDelay;
		readIdx2 = readIdx2 % maxDelay;
		
  		*out = buffer[ readIdx ] + fract * ( buffer[ readIdx2 ] - buffer[ readIdx ] ) ;
		lastOutput = *out++;
		++writeIndex;
//		++readIndex;
		writeIndex = writeIndex % maxDelay;
//		readIndex = readIndex % maxDelay;
	}
}
