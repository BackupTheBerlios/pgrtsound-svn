#include "TextFileOut.h"

using namespace std;

TextFileOut::TextFileOut() {
	type = "TextFileOut";
	iIn = addInput("input");
	pFreq = addParam("freq");
	setParam(pFreq, 1);
}

TextFileOut::~TextFileOut() {
    
}

void TextFileOut::process() {
	int n;
	float* in = input(iIn).signal;
	float pFreq1 = param(pFreq);
    
    ofstream out("TextFielOut.txt", ios_base::app);
    //assure(out, "TextFielOut.txt");
    
    
	for (n = 0; n < Module::framesPerBlock; n++) {
        if (n % int(pFreq1)==0)
    	   out << (*in++)  << endl;
	}
	
	out.close();
		
}
