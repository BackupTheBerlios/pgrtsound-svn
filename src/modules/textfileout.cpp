#include "TextFileOut.h"

using namespace std;

TextFileOut::TextFileOut() {
	type = "TextFileOut";
	iIn = AddInput("input");
	pFreq = AddParam("freq");
	
	frames = 0;
	SetParam(pFreq, 22050);
}

TextFileOut::~TextFileOut() {
}

void TextFileOut::Process() {
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
