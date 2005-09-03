#include "constant.h"

Constant::Constant() : Module("New constant"),
	oValue("wy"), pValue("value")
{
	AddOutput(oValue);
	AddParameter(pValue);
	oldValue = 0;
}

Constant::~Constant() {
}

void Constant::Process() {
	// jesli w tym bloku zmienila sie wartosc to wypelnij bufor wyjsciowy
	if (oldValue != pValue.GetValue())
	{
		unsigned long n;
		float value = pValue.GetValue();
		float *out = oValue.GetSignal();
			
		#ifndef NDEBUG
			cout << "Constant(" << GetName() << "): zmiana wartosci z " << oldValue
				<< " na " << value << endl;
		#endif
		
		for (n = 0; n < Module::framesPerBlock; n++) {
			*out++ = value;
		}
		oldValue = value;
	}
}
