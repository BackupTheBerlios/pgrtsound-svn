#include "delay.h"

Delay::Delay() : Module("New delay"),
	iIn("inpute"), oOut("output"), pDelay("delay")
 {
    //maksymalne opó?nienie w próbkach
    maxDelay = 400000;
	AddInput(iIn);
	AddOutput(oOut);
	pDelay.Bound(0, maxDelay, 1);    // ograczniczenie wartosci
	AddParameter(pDelay);
	
	buffor = new float[maxDelay];
	n1    = 0;
	n2    = n1 - (int)pDelay.GetValue();
}

Delay::~Delay() {
 delete buffor;
}

Module* Delay::Create(){
	return new Delay;
}

string Delay::GetTypeStatic() {
	return "delay";
}

string Delay::GetType() {
	return "delay";
}

void Delay::Process() {
	n2    = n1 - (int)pDelay.GetValue();
	
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();

    //cout << n2 <<"-" <<n1<<"=" << n2-n1 << " powinno byc " << pDelay.GetValue()<<endl;    
   	for(unsigned long n = 0; n < Module::framesPerBlock; n++) {
			buffor[n1] = (*in++);
			if (n2<0)
		      n2 = maxDelay + n2;
		    
	        *out++    = buffor[n2];
			if (n1>maxDelay) n1=0;
			if (n2>maxDelay) n2=0;
			n1++;
			n2++;
	}
}
