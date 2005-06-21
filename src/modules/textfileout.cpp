#include "TextFileOut.h"

using namespace std;

TextFileOut::TextFileOut() : Module("textfileout", "New text file"),
	iIn("input"), pFileName("filename"), pFreq("freq")
  {
	AddInput(iIn);
	AddParameter(pFileName);

	pFreq.SetValue(Module::sampleRate / 4.0f); // np. co 44100/4 = 11025 probek (1/4 sekundy)
	AddParameter(pFreq);

	frames = 0;
}

TextFileOut::~TextFileOut() {
}

void TextFileOut::Process() {
	float* in = iIn.GetSignal();
	float pFreq1 = pFreq.GetValue();
    
	ofstream out(pFileName.GetText().c_str(), ofstream::out | ofstream::app);

	for (int n = 0; n < Module::framesPerBlock; n++) {
		frames++;
        if (frames % (unsigned long)pFreq1 == 0)
    	   out << (*in++) << endl;
	}
	
	out.close();
}
