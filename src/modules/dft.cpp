#include "dft.h"

DFT::DFT() : Module("New delay"),
	iIn("inpute"), oOut("output") {
	AddInput(iIn);
	AddOutput(oOut);
	
	aa  = new float(Module::framesPerBlock);
	bb  = new float(Module::framesPerBlock);
	hfs = Module::framesPerBlock/2;
	pd  = M_PI/hfs;
}

DFT::~DFT() {
}

Module* DFT::Create(){
	return new DFT;
}

string DFT::GetTypeStatic() {
	return "dft";
}

string DFT::GetType() {
	return "dft";
}

void DFT::Process() {
	float* in = iIn.GetSignal();
	float* out = oOut.GetSignal();
	
    for(unsigned long i=0;i<Module::framesPerBlock;i++) {
        aa[i]=0;
        bb[i]=0;
    }
   
    //cout << n2 <<"-" <<n1<<"=" << n2-n1 << " powinno byc " << pDelay.GetValue()<<endl;    
   	for(unsigned long n = 0; n < Module::framesPerBlock; n++) {
	    float w=*in++;
        int im = n-hfs;
        for(unsigned long h=0;h<Module::framesPerBlock;h++) {
            float th=(pd*(h+1))*im;
            aa[h]+=w*cos(th);
            bb[h]+=w*sin(th);
        }
	}
	for (unsigned long h=0;h<Module::framesPerBlock;h++)
        *out++= sqrt(aa[h]*aa[h]+bb[h]*bb[h])/hfs;
}
