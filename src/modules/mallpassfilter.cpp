#include "mallpassfilter.h" 

MAllpassFilter::MAllpassFilter() :
	iIn( "Wejœæie audio" ),
	iFreq( "Czêstotliwoœæ [Hz]" ),
	oOut( "Wyjœcie audio" )
{
	AddInput(iIn);
	AddInput(iFreq);
	AddOutput(oOut);
	
	z = 0;
	d = 0;
    a = 0;
}


MAllpassFilter::~MAllpassFilter() {

}


void MAllpassFilter::Process() {
   	in = iIn.GetSignal();
	out = oOut.GetSignal();

    //frequency prewarping bo transformacja biliniowa
    freq = ( Module::sampleRate / M_PI ) * tan( M_PI * ( *iFreq.GetSignal() / Module::sampleRate ) );
    d = M_PI*( freq / Module::sampleRate );
    a = (1 - d) / (1 + d); // wspolczynniki filtru wszechprzepustowego

   	for( unsigned long i = 0; i < Module::framesPerBlock; i++ ) {
        y = a*( *in ) + z;
        z = a * y - ( *in++ );
        *out++ = y;
	}
}
