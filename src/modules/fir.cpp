#include "FIR.h"

FIR::FIR() : Module("Nowy FIR"),
	iIn("input"), oOut("output"), 
    b0("b0"), b1("b1"), b2("b2"), b3("b3"),b4("b4"),b5("b5"),
    b6("b6"), b7("b7"), b8("b8"), b9("b9"), b10("b10")
{
	AddInput(iIn);
	AddOutput(oOut);
	AddParameter(b0);
	AddParameter(b1);
	AddParameter(b2);
	AddParameter(b3);
	AddParameter(b4);
	AddParameter(b5);
	AddParameter(b6);
	AddParameter(b7);
	AddParameter(b8);
    AddParameter(b9);
    AddParameter(b10);
	
	//
	b0.Bound(1, 100, 0.1);
	b0.SetValue(1);
	b1.Bound(1, 100, 0.1);
	b1.SetValue(0);
	b2.Bound(1, 100, 0.1);
	b2.SetValue(0);
	b3.Bound(1, 100, 0.1);
	b3.SetValue(0);
	b4.Bound(1, 100, 0.1);
	b4.SetValue(0);
	b5.Bound(1, 100, 0.1);
	b5.SetValue(0);
	b6.Bound(1, 100, 0.1);
	b6.SetValue(0);
	b7.Bound(1, 100, 0.1);
	b7.SetValue(0);
	b8.Bound(1, 100, 0.1);
	b8.SetValue(0);
	b9.Bound(1, 100, 0.1);
	b9.SetValue(0);
	b10.Bound(1, 100, 0.1);
	b10.SetValue(0);
	
}

FIR::~FIR() {
}


void FIR::Init() {
	buffor = new float[10];
	n      = 0;
}

void FIR::Process()
{
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();
	
   	for(unsigned long i = 0; i < Module::framesPerBlock; i++) {
        for (int x = 9; x>1; x--) {
            buffor[x+1]=buffor[x];
            //cout << x << endl;
        }
        buffor[0] = (*in++);
	    
        *out++  = b0.GetValue() * buffor[0]+
                  b1.GetValue() * buffor[1]+
                  b2.GetValue() * buffor[2]+
                  b3.GetValue() * buffor[3]+
                  b4.GetValue() * buffor[4]+
                  b5.GetValue() * buffor[5]+
                  b6.GetValue() * buffor[6]+
                  b7.GetValue() * buffor[7]+
                  b8.GetValue() * buffor[8]+
                  b9.GetValue() * buffor[9]+
                  b10.GetValue()* buffor[10];
	}
}
