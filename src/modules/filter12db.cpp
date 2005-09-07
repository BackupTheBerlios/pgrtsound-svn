#include "Filter12dB.h"

Filter12dB::Filter12dB() :
	Module("Nowy filtr"),
	iIn("input"),
	oOut("output"),
	pFreq("frequency"),
	pFilterType("filter type")
{
	AddInput(iIn);
	
	AddOutput(oOut);
	
	AddParameter( pFreq );
	AddParameter( pFilterType );
	
	// wartosci domyslne
	pFilterType.SetText( "lp" );
	pFreq.Bound( 50, 0.75 * Module::sampleRate, 1 );
	pFreq.SetValue( 1000 );
	
	xn = yn = xn_1 = xn_2 = yn_1 = yn_2 = 0;
	Q = 1;
}

Filter12dB::~Filter12dB() {
}

void Filter12dB::Init() {
	UpdateCoefficients();
}

void Filter12dB::UpdateCoefficients() {
	w0 = 2 * M_PI * pFreq.GetValue() / Module::sampleRate;
    cos_w0 = cos(w0);
	sin_w0 = sin(w0);
	alpha = sin_w0 / ( 2 * Q );

	if( pFilterType.GetText() == "lp" ) {
		TRACE( "Filter12dB::UpdateCoefficients - Low pass %.2f Hz\n", pFreq.GetValue() );
		
		b0 = ( 1 - cos_w0 ) / 2;
        b1 = 1 - cos_w0;
        b2 = (1 - cos_w0) / 2;
        a0 = 1 + alpha;
        a1 = -2 * cos_w0;
        a2 = 1 - alpha;

		B0 = b0/a0; B1 = b1/a0; B2 = b2/a0;
		A1 = a1/a0; A2 = a2/a0;

		return;
	}

	if( pFilterType.GetText() == "hp" ) {
		TRACE( "Filter12dB::UpdateCoefficients - High pass %.2f Hz\n", pFreq.GetValue() );

		b0 =  ( 1 + cos_w0 ) / 2;
        b1 = -( 1 + cos_w0 );
        b2 =  ( 1 + cos_w0 ) / 2;
		a0 = 1 + alpha;
		a1 =  -2 * cos_w0;
		a2 = 1 - alpha;
		
		B0 = b0/a0; B1 = b1/a0; B2 = b2/a0;
		A1 = a1/a0; A2 = a2/a0;

  		return;
	}
}

void Filter12dB::Process() {
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();
	
   	for( unsigned long i = 0; i < Module::framesPerBlock; i++ ) {
	    xn = *in++;

    	yn = B0*xn + B1*xn_1 + B2*xn_2 - A1*yn_1 - A2*yn_2;

	    xn_2 = xn_1;
	    xn_1 = xn;
	    yn_2 = yn_1;
	    yn_1 = yn;

	    *out++ = yn;
	}
}
