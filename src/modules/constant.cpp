#include "constant.h"

Constant::Constant() {
	type = "constant";
	oldValue = 0;
	oValue = AddOutput("value");
	pValue = AddParam("value");
}

Constant::~Constant() {
}

void Constant::Process() {
	// jesli w tym bloku zmienila sie wartosc to wypelnij bufor wyjsciowy
	if (oldValue != params[pValue]->value)
	{
		int n;
		float value = params[pValue]->value;
		float *out = outputs[oValue]->signal;
			
		#ifndef NDEBUG
			cout << "Constant(" << id << "): zmiana wartosci z " << oldValue
				<< " na " << value << endl;
		#endif
		
		for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = value;
		}
		oldValue = value;
	}
}
