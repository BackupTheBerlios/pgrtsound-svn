#ifndef TextFileOut_H
#define TextFileOut_H

#include "module.h"
#include <fstream>
#include <iostream>

using namespace std;
/**
 * Modul mnozenia. Mnozy skalarnie dwa sygnaly wejsciowe.
 */
class TextFileOut : public Module
{
	public:
		TextFileOut();
		~TextFileOut();
		void process();
	
	private:
		int iIn, pFreq;
};

#endif // MUL_H
