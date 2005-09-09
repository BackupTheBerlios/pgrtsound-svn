#include "msinlfo.h"

MSinLFO::MSinLFO() :
	Module( "New LFO Sin" ),
	pFreq( "Czêstotliwoœæ" ),
	oOut( "LFO output" )
{
	AddParameter( pFreq );
	AddOutput( oOut );
	
	pFreq.Bound( 1, 200, 0.1 ); // TODO: ustalic zakres czest. ???
	pFreq.SetValue( 5 );
}

MSinLFO::~MSinLFO() {
}

void MSinLFO::Init() {
	sinZ = 0.0f;
	cosZ = 1.0f;
	Update();
}

void MSinLFO::Update() {
	freq = M_2PI * pFreq.GetValue() / Module::sampleRate;
	
	//TRACE( "MSinLFO::Update - Czestotliwosc %f\n", freq );

}

void MSinLFO::Process() {
	float* out = oOut.GetSignal();

	for( unsigned long i = 0; i < Module::framesPerBlock; i++ ) {
		sinZ = sinZ + freq * cosZ;
		cosZ = cosZ - freq * sinZ;
		*out++ = sinZ;
	}
}
