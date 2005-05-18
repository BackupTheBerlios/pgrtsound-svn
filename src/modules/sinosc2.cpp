#include "sinosc2.h"
SinOsc2::SinOsc2() {
	float	*sample, tableSizeInverted;
	tableSize = 16384;

	type = "osc2";

	iFreq.SetName("frequency");
	AddInput(&iFreq);
	
	iAmp.SetName("amplitude");
	AddInput(&iAmp);
	
	oOut.SetName("output");
 	AddOutput(&oOut);

	sinTable = new float[tableSize + 1];
	sample = sinTable;
    tableSizeInverted = 1.0/(float)tableSize;
	scale = (float)tableSize / (float)Module::sampleRate;
	tableRingMask = tableSize - 1;

	// generowanie tablicy
	for(long n = 0; n < tableSize; n++) {
		*sample = sin(M_2PI*(float)n*tableSizeInverted);
		sample++;
	}

	phase = 0;
	tablePosition = 0;
}

SinOsc2::~SinOsc2() {
	delete sinTable;
}

void SinOsc2::Process() {
	float* freq = iFreq.GetSignal();
	float* amp = iAmp.GetSignal();
	float* out = oOut.GetSignal();

	for(int i = 0; i < Module::framesPerBlock; i++) {
		phaseFloor = floorf(phase);
		phaseFraction = phase - phaseFloor;

		// zawijanie pozycji
		tablePosition = long(phaseFloor) & tableRingMask;
		tempSample = &sinTable[tablePosition];

		// wyjscie z interpolcja liniowa
		*out++ = *amp++ * (*tempSample + phaseFraction*( *(tempSample + 1) - *tempSample ));
		
       	phase = tablePosition + (*freq++ * scale);
	}
}
