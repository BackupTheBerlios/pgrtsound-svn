#ifndef TextFileOut_H
#define TextFileOut_H

#include "module.h"
#include <fstream>
#include <iostream>

using namespace std;

/**
 * Modul zapisu do pliku. Dane dopisywane s� do pliku FileTexOut.txt
 */
class TextFileOut : public Module
{
	public:
		TextFileOut();
		~TextFileOut();
		void Process();
			static Module* Create();
	private:
		ofstream out;
        unsigned long frames;
		ParameterString pFileName;
		ParameterFloat pFreq;
		Input iIn;
};

#endif // MUL_H
