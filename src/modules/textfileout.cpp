#include "TextFileOut.h"

using namespace std;

TextFileOut::TextFileOut() {
	type = "TextFileOut";
	iIn = addInput("input");
	pFreq = addParam("freq");
	
	frames = 0;
	setParam(pFreq, 22050);
}

TextFileOut::~TextFileOut() {
}

void TextFileOut::process() {
	int n;
	float* in = inputs[iIn]->signal;
	float pFreq1 = params[pFreq]->value;
    
    ofstream out("TextFileOut.txt", ios_base::app);
    //assure(out, "TextFileOut.txt");

	for (n = 0; n < Module::framesPerBlock; n++) {
		frames++;
        if (frames % (unsigned long)pFreq1 == 0)
    	   out << (*in++)  << endl;
	}
	
	out.close();
}
