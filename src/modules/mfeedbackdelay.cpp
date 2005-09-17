#include "mfeedbackdelay.h"

MFeedbackDelay::MFeedbackDelay() :
	Module( "Feedback Delay" ),
	iIn("input"),
	oOut("output"),
	pDelay( "OpóŸnienie" ),
	pFeedback("Wsp. sprzê¿enia zwr.")
{
	AddInput( iIn );
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

MFeedbackDelay::~MFeedbackDelay() {
}

void MFeedbackDelay::Init() {
	ChangeMaxDelay();
}

void MFeedbackDelay::SampleRateChanged() {
   	pDelay.Bound( 0, (unsigned long)Module::sampleRate, 1 );
    ChangeMaxDelay();
}

void MFeedbackDelay::ChangeMaxDelay() {
	delete []buffer;
	maxDelay = (unsigned long)Module::sampleRate; // maksymalne opoznienie
	buffer = new float[ maxDelay ];

	// zerowanie bufora
	for( unsigned long i = 0; i < maxDelay; i++ )
		buffer[i] = 0.0f;

	writeIndex = 0;
	
	SomethingChanged();
}

void MFeedbackDelay::SomethingChanged() {
	delay = (unsigned long)( pDelay.GetValue() * Module::sampleRate * 0.001f );
	delay = ( delay > maxDelay )? maxDelay : delay;

	feedback = pFeedback.GetValue();
	feedback = ( feedback > 0.99f ) ? 0.99f : feedback;

	readIndex = writeIndex + (maxDelay - delay);
	readIndex = readIndex % maxDelay;

	TRACE( "MFeedbackDelay::SomethingChanged - delay = %i, feedback = %f\n",
	    delay, feedback );
}

void MFeedbackDelay::Process() {
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();

   	for( unsigned long i = 0; i < Module::framesPerBlock; i++ ) {
		buffer[ writeIndex ] = *in++ + lastOutput * feedback;
		*out = buffer[ readIndex ];
		lastOutput = *out++;
		++writeIndex;
		++readIndex;
		writeIndex = writeIndex % maxDelay;
		readIndex = readIndex % maxDelay;
	}
}
