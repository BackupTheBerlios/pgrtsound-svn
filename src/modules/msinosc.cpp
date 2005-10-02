#include "msinosc.h"

MSinOsc::MSinOsc() :
    iFreq("Czêstotliwoœæ"),
	iAmp("Amplituda"),
	oOut( "Audio" )
{
	AddInput( iFreq );
	AddInput( iAmp );
	AddOutput( oOut );
}

MSinOsc::~MSinOsc() {
}

void MSinOsc::Init() {
	sinZ = 0.0f;
	cosZ = 1.0f; // wzbudzenie filtru IIR
	freqCoeff = M_2PI / Module::sampleRate;
}

void MSinOsc::SampleRateChanged() {
	freqCoeff = M_2PI / Module::sampleRate;
}

void MSinOsc::Process() {
	amp = iAmp.GetSignal();
	out = oOut.GetSignal();
	freqIn = iFreq.GetSignal();

	for( unsigned long i = 0; i < Module::framesPerBlock; i++ ) {
		freq = ( *freqIn++ ) * freqCoeff;
		sinZ = sinZ + freq * cosZ;
		cosZ = cosZ - freq * sinZ;
		*out++ = ( *amp++ ) * sinZ;
	}
}
