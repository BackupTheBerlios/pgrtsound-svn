#include "Filter12dB.h"

Filter12dB::Filter12dB() : Module("filter12db", "Nowy filtr Butterwortha"),
	iIn("input"), oOut("output"), pFreq("frequency")
{
	AddInput(iIn);
	AddOutput(oOut);
	AddParameter(pFreq);
	
	pFreq.Bound(100, 15000, 10);
	pFreq.SetValue(1000);
}

Filter12dB::~Filter12dB() {
}

void Filter12dB::Init() {
	frequency = pFreq.GetValue();
	omega = 2.0*M_PI*frequency / Module::sampleRate;

	// prewarping czestotliwosci
	K = tan(omega/2);
	Q = 1;
	V = 1;
	// wspolczynniki licznika transformaty H(z)
	b0 = K*K;
	b1 = 2*b0;
	b2 = b0;
	// wspolczynniki mianownika transformaty H(z)
	a0 = 1 + K/Q + b0;
	a1 = 2*(b0 - 1);
	a2 = 1 - K/Q + b0;
	
	// normalizacja wspolczynnikow wzgledem a0
	a0inv = 1/a0;
	b0 = b0*a0inv; b1 = b1*a0inv; b2 = b2*a0inv;
	a1 = a1*a0inv; a2 = a2*a0inv; a0 = 1;
	
	// bufory
	z1m = 0;
	z2m = 0;
}

void Filter12dB::Process()
{
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();
	
   	for(int i = 0; i < Module::framesPerBlock; i++) {
	    xn = *in++;
	    yn = b0*xn + z1m;
	    z2n = b2*xn - a2*yn;
	    z1n = z2m + b1*xn - a1*yn;
	    z1m = z1n;
	    z2m = z2n;
	    *out++ = 0.5*yn;
	}
}
