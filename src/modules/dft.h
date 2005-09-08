#ifndef DFT_H
#define DFT_H

#include "module.h"
#include <math.h>

/**
 Modul DFT
 z wykorzystaniem sin i cos wiec wolno poki co
*/
class DFT : public Module
{
	public:
		DFT();
		~DFT();
		void Process();
		static Module* Create();
		static string GetTypeStatic();
		string GetType();
		
	private:
		Input           iIn;
		Output          oOut;
		float           *aa;
        float           *bb;
        int             hfs;
        float           pd;
};

#endif // MUL_H
