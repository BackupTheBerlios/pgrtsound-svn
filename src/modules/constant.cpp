#include "constant.h"

// class constructor
Constant::Constant() {
	type = "constant";
	oldValue = 0;
	oValue = addOutput("value");
	pValue = addParam("value");
}

// class destructor
Constant::~Constant() {
}

void Constant::process() {
	if (oldValue != param(pValue))
	{
		int n;
		float value = param(pValue);
		float *out = output(oValue).signal;
			
		#ifndef NDEBUG
			cout << "Constant(" << id << "): zmiana wartosci z " << oldValue
				<< " na " << value << endl;
		#endif
		
		for (n = 0; n < Module::framesPerBlock; n++)
		{
			*out++ = value;
		}
		oldValue = value;
	}
}
