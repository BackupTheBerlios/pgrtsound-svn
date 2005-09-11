#include "delay.h"

Delay::Delay() : Module("New delay"),
	iIn("inpute"), oOut("output"), pDelay("delay")
 {
    //maksymalne opóŸnienie w próbkach
	AddInput(iIn);
	AddOutput(oOut);
	pDelay.Bound(1, 400000, 1);    // ograczniczenie wartosci
	AddParameter(pDelay);
	
}

Delay::~Delay() {

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
	n    = (int)pDelay.GetValue();
	
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();


   	for(unsigned long i = 0; i < Module::framesPerBlock; i++) {
            if (n<1) {
                *out++ = *in++;   
            } else {
		       if (n <= buffor.size()) {
    	           *out++    = buffor.at(0);
                   buffor.pop_front();
    	           buffor.push_back(*in++); 
                } else {
                    *out++  = 0;
    	           buffor.push_back(*in++);
                }
            }
	}
}
