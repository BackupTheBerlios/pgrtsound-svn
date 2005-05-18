#include "TextFileOut.h"

using namespace std;

TextFileOut::TextFileOut() {
	type = "TextFileOut";
	
	iIn.SetName("input");
	AddInput(&iIn);
	
	pFileName.SetName("filename");
	pFileName.SetGUIType(gtEditBox);
	AddParameter(&pFileName);
	
	pFreq.SetName("freq");
	pFreq.SetGUIType(gtSlider);
	pFreq.SetValue(22050);
	AddParameter(&pFreq);

	frames = 0;
}

TextFileOut::~TextFileOut() {
}

void TextFileOut::Process() {
	int n;
	float* in = iIn.GetSignal();
	float pFreq1 = pFreq.GetValue();
    
    ofstream out(pFileName.GetText().c_str(), ios_base::app);
    //assure(out, "TextFileOut.txt");

	for (n = 0; n < Module::framesPerBlock; n++) {
		frames++;
        if (frames % (unsigned long)pFreq1 == 0)
    	   out << (*in++)  << endl;
	}
	
	out.close();
}
