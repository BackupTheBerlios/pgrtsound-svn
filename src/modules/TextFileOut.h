#ifndef TextFileOut_H
#define TextFileOut_H

#include "module.h"
#include <fstream>
#include <iostream>

using namespace std;
/**
 * Modul zapisu do pliku. Dane dopisywane s¹ do pliku FileTexOut.txt
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
